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
    struct libfc_template_group_t    *tg;
    /** rw cond mutex */
    pthread_mutex_t         mux;
    /** write ready */
    pthread_cond_t          wok;
    /** read ready */
    pthread_cond_t          rok;
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
libftrace_t *ftrace_create(const char *filename, int version, const char *lpfilename)
{
    libftrace_t *ft = NULL;
    int pterrno = 0;
    
    /* create structure */
    ft = malloc(sizeof(*ft));
    memset(ft, 0, sizeof(*ft));
    ft->filename = filename;

    /* create condition variables and mutex */
    if ((pterrno = pthread_mutex_init(&ft->mux, NULL)) != 0) {
        fprintf(stderr, "couldn't init pthread mux: %s\n", strerror(pterrno));
        goto err;
    }

    if ((pterrno = pthread_cond_init(&ft->wok, NULL)) != 0) {
        fprintf(stderr, "couldn't init pthread wok: %s\n", strerror(pterrno));
        goto err;
    }

    if ((pterrno = pthread_cond_init(&ft->rok, NULL)) != 0) {
        fprintf(stderr, "couldn't init pthread rok: %s\n", strerror(pterrno));
        goto err;
    }
    
    /* initialize logging if necessary */
    if (lpfilename) {
        libfc_initialize_logging(lpfilename);
    }

    /* open underlying source */
    if (!(ft->wio = wandio_create(filename))) {
        /* FIXME error handling */
        fprintf(stderr, "couldn't open %s: %s", filename, strerror(errno));
        goto err;
    }

    /* create a template set -- this will be populated once we decide what to collect */
    ft->tg = libfc_template_group_new(version);

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
        pthread_cond_destroy(&ft->rok);
        pthread_mutex_destroy(&ft->mux);

        if (ft->tg) libfc_template_group_delete(ft->tg);
        if (ft->wio) wandio_destroy(ft->wio);
        free(ft);
    }
}

static int _ftrace_semcb_inner(libftrace_t *ft)
{
    /* check terminate signal */
    if (ft->terminate) {
        fprintf(stderr,"_ftrace_semcb_inner() telling placement collector to stop.\n");
        return 0;
    }

    /* signal read ready, outer thread will now return a record */
    pthread_mutex_lock(&ft->mux);
    pthread_cond_signal(&ft->rok);
//    fprintf(stderr,"_ftrace_semcb_inner() signaled rok, waiting for wok.\n");

    /* wait write ready, since next placement will happen after return */
    pthread_cond_wait(&ft->wok, &ft->mux);
    pthread_mutex_unlock(&ft->mux);
//    fprintf(stderr,"_ftrace_semcb_inner() got wok.\n");
    
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

    fprintf(stderr, "reader thread starting\n");
    
    /* run the placement collector in the subordinate thread */
    rv = libfc_collect_from_wandio(ft->wio, ft->filename, ft->tg);

    /* set valid flag based on rv */
    if (rv) {
        ft->valid = 0; /* eof */
    } else {
        ft->valid = -1; /* error */
    }
    
    fprintf(stderr, "reader thread exiting, return value %d\n", rv);

    /* signal read ready, outer thread will react to invalid record. */
    pthread_mutex_lock(&ft->mux);
    pthread_cond_signal(&ft->rok);
    pthread_mutex_unlock(&ft->mux);
    fprintf(stderr,"_ftrace_rthread() signaled rok on exit.\n");
    
    /* we don't care about the return */
    return NULL;
}

/** Start reading uniflows from a libftrace source */
libftrace_uniflow_t *ftrace_start_uniflow(libftrace_t *ft)
{
    int pterrno = 0;
    struct libfc_template_t *t;
    
    /* check to see if we've already registered a uniflow */
    if (ft->uf._ft == ft) return &ft->uf;
    
    /* register base template v4 */
    t = libfc_template_new(ft->tg);
    libfc_register_placement(t, "flowStartMilliseconds", 
        &ft->uf.time_start, sizeof(ft->uf.time_start));
    libfc_register_placement(t, "flowEndMilliseconds", 
        &ft->uf.time_end, sizeof(ft->uf.time_end));
    libfc_register_placement(t, "packetDeltaCount", 
        &ft->uf.packets, sizeof(ft->uf.packets));
    libfc_register_placement(t, "octetDeltaCount", 
        &ft->uf.octets, sizeof(ft->uf.octets));
    libfc_register_placement(t, "sourceIPv4Address", 
        &ft->uf.ip.v4.src, sizeof(ft->uf.ip.v4.src));
    libfc_register_placement(t, "destinationIPv4Address", 
        &ft->uf.ip.v4.dst, sizeof(ft->uf.ip.v4.dst));
    libfc_register_placement(t, "sourceTransportPort",
        &ft->uf.port_src, sizeof(ft->uf.port_src));
    libfc_register_placement(t, "destinationTransportPort",
        &ft->uf.port_dst, sizeof(ft->uf.port_dst));
    libfc_register_placement(t, "protocolIdentifier",
        &ft->uf.ip_proto, sizeof(ft->uf.ip_proto));
    libfc_register_callback(t, _ftrace_semcb_v4, ft);

    /* register base template v6 */
    t = libfc_template_new(ft->tg);
    libfc_register_placement(t, "flowStartMilliseconds", 
        &ft->uf.time_start, sizeof(ft->uf.time_start));
    libfc_register_placement(t, "flowEndMilliseconds", 
        &ft->uf.time_end, sizeof(ft->uf.time_end));
    libfc_register_placement(t, "packetDeltaCount", 
        &ft->uf.packets, sizeof(ft->uf.packets));
    libfc_register_placement(t, "octetDeltaCount", 
        &ft->uf.octets, sizeof(ft->uf.octets));
    libfc_register_placement(t, "sourceIPv6Address", 
        &ft->uf.ip.v4.src, sizeof(ft->uf.ip.v4.src));
    libfc_register_placement(t, "destinationIPv6Address", 
        &ft->uf.ip.v4.dst, sizeof(ft->uf.ip.v4.dst));
    libfc_register_placement(t, "sourceTransportPort",
        &ft->uf.port_src, sizeof(ft->uf.port_src));
    libfc_register_placement(t, "destinationTransportPort",
        &ft->uf.port_dst, sizeof(ft->uf.port_dst));
    libfc_register_placement(t, "protocolIdentifier",
        &ft->uf.ip_proto, sizeof(ft->uf.ip_proto));
    libfc_register_callback(t, _ftrace_semcb_v6, ft);

    /* FIXME more templates */

    /* make the reference circular so we know we started a uniflow */
    ft->uf._ft = ft;
    
    /* then start the reader thread */
    if ((pterrno = pthread_create(&ft->rt, NULL, _ftrace_rthread, ft)) != 0) {
        fprintf(stderr, "couldn't start reader thread: %s\n", strerror(pterrno));
        return NULL;
    }

    return &ft->uf;
}

/** Stop reading a uniflow source */
void ftrace_destroy_uniflow(libftrace_uniflow_t *uf) {
    if (!uf->_ft->terminate) {
        /* signal inner uniflow to stop reading unless eof */
        pthread_mutex_lock(&uf->_ft->mux);
        uf->_ft->terminate++;
        pthread_cond_signal(&uf->_ft->wok);
        pthread_mutex_unlock(&uf->_ft->mux);
    }

    if (pthread_join(uf->_ft->rt, NULL) != 0) {
        /* FIXME error */
        fprintf(stderr,"error joining reader thread\n");
    }
}

/** Read the next uniflow from a libftrace reader. 
    Skips records in the stream which do not match uniflows. */
int ftrace_next_uniflow(libftrace_uniflow_t *uf) {
    
    /* signal write ready */
    pthread_mutex_lock(&uf->_ft->mux);
    pthread_cond_signal(&uf->_ft->wok);
//    fprintf(stderr,"next_uniflow() signaled wok, waiting for rok.\n");

    /* wait read ready */
    pthread_cond_wait(&uf->_ft->rok, &uf->_ft->mux);
    pthread_mutex_unlock(&uf->_ft->mux);
//    fprintf(stderr,"next_uniflow() got rok.\n");

    /* check valid */
    return uf->_ft->valid;
}

int ftrace_add_specfile(libftrace_t *ft, const char *specfilename) {
    return libfc_add_specfile(specfilename);
}
