/* Hi Emacs, please use -*- mode: C; -*- */
/* Copyright (c) 2011-2014 ETH Zürich. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions are met:
 *    * Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *    * Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *    * Neither the names of ETH Zürich nor the names of other contributors 
 *      may be used to endorse or promote products derived from this software 
 *      without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL ETH
 * ZÜRICH BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 */

#include "libftrace.h"
#include "libfc.h"

#include <wandio.h>
#include <pthread.h>

#include <string.h>
#include <errno.h>


/**
 * @file libftrace.c
 * @brief Implementation of libtrace-like interface to IPFIX uniflow records
 * @author Brian Trammell <trammell@tik.ee.ethz.ch>
 *
 */

struct libftrace_st {
    /** libwandio source */
    io_t                    *wio;
    /** source name */
    const char              *filename;
    /** template set for callbacks */
    struct libfc_template_set_t    *ts;
    /** write ready */
    pthread_cond_t          wok;
    pthread_mutex_t         wmx;
    /** read ready */
    pthread_cond_t          rok;
    pthread_mutex_t         rmx;
    /** reader thread */
    pthread_t               rt;
    /* storage for uniflow */
    libftrace_uniflow_t     uf;
    /** valid record flag */
    int                     valid;
    /* abort flag */
    int                     terminate;
};

/** Open a libftrace source on an IPFIX/PDU file */
libftrace_t *ftrace_create(const char *filename)
{
    libftrace_t *ft = NULL;

    /* create structure */
    ft = malloc(sizeof(*ft));
    memset(ft, 0, sizeof(*ft));
    ft->filename = filename;

    /* create condition variables and mutexes */
    if (!(pthread_cond_init(&ft->wok, NULL))) {
        fprintf(stderr, "couldn't init pthread wok: %s\n", strerror(errno));
        goto err;
    }
    
    if (!(pthread_mutex_init(&ft->wmx, NULL))) {
        fprintf(stderr, "couldn't init pthread wmx: %s\n", strerror(errno));
        goto err;
    }

    if (!(pthread_cond_init(&ft->rok, NULL))) {
        fprintf(stderr, "couldn't init pthread rok: %s\n", strerror(errno));
        goto err;
    }
    
    if (!(pthread_mutex_init(&ft->rmx, NULL))) {
        fprintf(stderr, "couldn't init pthread rmx: %s\n", strerror(errno));
        goto err;
    }

    /* open underlying source */
    if (!(ft->wio = wandio_create(filename))) {
        /* FIXME error handling */
        fprintf(stderr, "couldn't open %s: %s", filename, strerror(errno));
        goto err;
    }

    /* create a template set -- this will be populated once we decide what to collect */
    ft->ts = ipfix_template_set_new();

    /* all done */
    return ft;

err:
    ftrace_destroy(ft);
    return NULL;
}

/** Close a libftrace source */
void ftrace_destroy(libftrace_t *ft)
{
    /* FIXME need to destroy uniflow if currently reading */
    if (ft) {
        pthread_cond_destroy(&ft->wok);
        pthread_mutex_destroy(&ft->wmx);
        pthread_cond_destroy(&ft->rok);
        pthread_mutex_destroy(&ft->rmx);

        if (ft->ts) ipfix_template_set_delete(ft->ts);
        if (ft->wio) wandio_destroy(ft->wio);
        free(ft);
    }
}

static int _ftrace_semcb_inner(libftrace_t *ft)
{
    /* check terminate signal */
    if (ft->terminate) return 0;

    /* signal read ready, outer thread will now return a record */
    pthread_mutex_lock(&ft->rmx);
    pthread_cond_signal(&ft->rok);
    pthread_mutex_unlock(&ft->rmx);
    
    /* wait write ready, since next placement will happen after return */
    pthread_mutex_lock(&ft->wmx);
    pthread_cond_wait(&ft->wok, &ft->wmx);
    pthread_mutex_unlock(&ft->wmx);
    
    /* tell placement collector to keep going */
    return 1;
}

static int _ftrace_semcb_v4(const struct libfc_template_t *t, void *vpft) {
    libftrace_t *ft = (libftrace_t *)vpft;

    /* set version */
    ft->uf.ip_ver = 4;

    /* signal flow ready */
    return _ftrace_semcb_inner(ft);
}

static int _ftrace_semcb_v6(const struct libfc_template_t *t, void *vpft) {
    libftrace_t *ft = (libftrace_t *)vpft;

    /* set version */
    ft->uf.ip_ver = 6;

    /* signal flow ready */
    return _ftrace_semcb_inner(ft);
}

void *_ftrace_rthread(void *vpft) {
    libftrace_t *ft = (libftrace_t *)vpft;
    int rv;

    /* signal valid content */
    ft->valid = 1;

    /* run the placement collector in the subordinate thread */
    rv = ipfix_collect_from_wandio(ft->wio, ft->filename, ft->ts);

    /* set valid flag based on rv */
    if (rv) {
        ft->valid = 0; /* eof */
    } else {
        ft->valid = -1; /* error */
    }
    
    /* we don't care about the return */
    return NULL;
}

/** Start reading uniflows from a libftrace source */
libftrace_uniflow_t *ftrace_start_uniflow(libftrace_t *ft)
{
    struct libfc_template_t *t;
    
    /* check to see if we've already registered a uniflow */
    if (ft->uf._ft == ft) return &ft->uf;

    /* register base template v4 */
    t = ipfix_template_new(ft->ts);
    ipfix_register_placement(t, "flowStartMilliseconds", 
        &ft->uf.time_start, sizeof(ft->uf.time_start));
    ipfix_register_placement(t, "flowEndMilliseconds", 
        &ft->uf.time_end, sizeof(ft->uf.time_end));
    ipfix_register_placement(t, "packetDeltaCount", 
        &ft->uf.packets, sizeof(ft->uf.packets));
    ipfix_register_placement(t, "octetDeltaCount", 
        &ft->uf.octets, sizeof(ft->uf.octets));
    ipfix_register_placement(t, "sourceIPv4Address", 
        &ft->uf.ip.v4.src, sizeof(ft->uf.ip.v4.src));
    ipfix_register_placement(t, "destinationIPv4Address", 
        &ft->uf.ip.v4.dst, sizeof(ft->uf.ip.v4.dst));
    ipfix_register_placement(t, "sourceTransportPort",
        &ft->uf.port_src, sizeof(ft->uf.port_src));
    ipfix_register_placement(t, "destinationTransportPort",
        &ft->uf.port_dst, sizeof(ft->uf.port_dst));
    ipfix_register_placement(t, "protocolIdentifier",
        &ft->uf.ip_proto, sizeof(ft->uf.ip_proto));
    libfc_register_callback(t, _ftrace_semcb_v4, ft);

    /* register base template v6 */
    t = ipfix_template_new(ft->ts);
    ipfix_register_placement(t, "flowStartMilliseconds", 
        &ft->uf.time_start, sizeof(ft->uf.time_start));
    ipfix_register_placement(t, "flowEndMilliseconds", 
        &ft->uf.time_end, sizeof(ft->uf.time_end));
    ipfix_register_placement(t, "packetDeltaCount", 
        &ft->uf.packets, sizeof(ft->uf.packets));
    ipfix_register_placement(t, "octetDeltaCount", 
        &ft->uf.octets, sizeof(ft->uf.octets));
    ipfix_register_placement(t, "sourceIPv6Address", 
        &ft->uf.ip.v4.src, sizeof(ft->uf.ip.v4.src));
    ipfix_register_placement(t, "destinationIPv6Address", 
        &ft->uf.ip.v4.dst, sizeof(ft->uf.ip.v4.dst));
    ipfix_register_placement(t, "sourceTransportPort",
        &ft->uf.port_src, sizeof(ft->uf.port_src));
    ipfix_register_placement(t, "destinationTransportPort",
        &ft->uf.port_dst, sizeof(ft->uf.port_dst));
    ipfix_register_placement(t, "protocolIdentifier",
        &ft->uf.ip_proto, sizeof(ft->uf.ip_proto));
    libfc_register_callback(t, _ftrace_semcb_v6, ft);

    /* FIXME more templates */

    /* then start the reader thread */
    if (pthread_create(&ft->rt, NULL, _ftrace_rthread, ft) != 0) {
        /* FIXME error */
    }

    return &ft->uf;
}

/** Stop reading a uniflow source */
void ftrace_destroy_uniflow(libftrace_uniflow_t *uf) {
    if (!uf->_ft->terminate) {
        /* signal inner uniflow to stop reading unless eof */
        pthread_mutex_lock(&uf->_ft->wmx);
        uf->_ft->terminate++;
        pthread_cond_signal(&uf->_ft->wok);
        pthread_mutex_unlock(&uf->_ft->wmx);
    }

    if (pthread_join(uf->_ft->rt, NULL) != 0) {
        /* FIXME error */
    }
}

/** Read the next uniflow from a libftrace reader. 
    Skips records in the stream which do not match uniflows. */
int ftrace_next_uniflow(libftrace_uniflow_t *uf) {

    /* signal write ready */
    pthread_mutex_lock(&uf->_ft->wmx);
    pthread_cond_signal(&uf->_ft->wok);
    pthread_mutex_unlock(&uf->_ft->wmx);

    /* wait read ready */
    pthread_mutex_lock(&uf->_ft->rmx);
    pthread_cond_wait(&uf->_ft->rok, &uf->_ft->rmx);
    pthread_mutex_unlock(&uf->_ft->rmx);

    /* check valid */
    return uf->_ft->valid;
}