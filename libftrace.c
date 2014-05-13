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
#include "ipfix.h"

#include <wandio.h>

#include <pthread.h>

/**
 * @file libftrace.c
 * @brief Implementation of libtrace-like interface to IPFIX uniflow records
 * @author Brian Trammell <trammell@tik.ee.ethz.ch>
 *
 */

struct libftrace_st {
    /** libwandio source */
    io_t                    *wio;
    /** template set for callbacks */
    ipfix_template_set_t    *ts;
    /** write ready */
    pthread_cond_t          wok;
    /** read ready */
    pthread_cond_t          rok;
    /* storage for uniflow */
    libftrace_uniflow_t     uf;

}

/** Open a libftrace source on an IPFIX/PDU file */
libftrace_t *ftrace_create(const char *filename)
{
    libftrace_t *ft = NULL;

    /* create structure */
    ft = malloc(sizeof(*ft));
    memset(ft, 0, sizeof(*ft));

    /* create condition variables */
    if (!(pthread_cont_init(&ft->wok, NULL))) {
        fprintf(stderr, "couldn't init pthreads: %s\n", strerror(errno));
    }
    if (!(pthread_cont_init(&ft->rok, NULL))) {
        fprintf(stderr, "couldn't init pthreads: %s\n", strerror(errno));
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
}

/** Close a libftrace source */
void ftrace_destroy(libftrace_t *ft)
{
    if (ft) {
        pthread_cond_destroy(&ft->wok);
        pthread_cond_destroy(&ft->rok);
        if (ft->ts) ipfix_template_set_delete(ft->ts);
        if (ft->wio) wandio_destroy(ft->wio);
        free(ft);
    }
}

static void _ftrace_semcb_inner(const libftrace_t *ft)
{
    /* signal read ready, outer thread will now return a record */
    pthread_cond_signal(&ft->rok);

    /* wait write ready, since next placement will happen after return */
    pthread_cond_wait(&ft->wok);
}

static void _ftrace_semcb_v4(const struct ipfix_template_t *t, void *vpft) {
    libftrace_t *ft = (libftrace_t *)vpft;

    /* set version */
    ft->uf.ip_ver = 4;

    /* signal flow ready */
    _ftrace_semcb_inner(ft)
}

static void _ftrace_semcb_v6(const struct ipfix_template_t *t, void *vpft) {
    libftrace_t *ft = (libftrace_t *)vpft;

    /* set version */
    ft->uf.ip_ver = 6;

    /* signal flow ready */
    _ftrace_semcb_inner(ft)
}

void _ftrace_rthread(void *vpft) {
    libftrace_t *ft = (libftrace_t *)vpft;
    int rv;

    /* signal valid content */
    ft->_valid = 1;

    /* run the placement collector in the subordinate thread */
    rv = ipfix_collect_from_wandio(io_t wio, struct ipfix_template_t* t));

    /* set valid flag based on rv */
    if (rv) {
        ft->_valid = 0; /* eof */
    } else {
        ft->_valid = -1; /* error */
    }
}

/** Start reading uniflows from a libftrace source */
libftrace_uniflow_t *ftrace_start_uniflow(libftrace_t *ft)
{
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
        &ft->uf.ip.v4.dst, sizeof(ft->uf.ip.v4.dst);
    ipfix_register_placement(t, "sourceTransportPort",
        &ft->uf.port_src, sizeof(ft->uf.port_src));
    ipfix_register_placement(t, "destinationTransportPort",
        &ft->uf.port_dst, sizeof(ft->uf.port_dst));
    ipfix_register_placement(t, "protocolIdentifier",
        &ft->uf.ip_proto, sizeof(ft->uf.ip_proto));
    ipfix_register_callback(t, _ftrace_semcb_v4, ft);

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
        &ft->uf.ip.v4.dst, sizeof(ft->uf.ip.v4.dst);
    ipfix_register_placement(t, "sourceTransportPort",
        &ft->uf.port_src, sizeof(ft->uf.port_src));
    ipfix_register_placement(t, "destinationTransportPort",
        &ft->uf.port_dst, sizeof(ft->uf.port_dst));
    ipfix_register_placement(t, "protocolIdentifier",
        &ft->uf.ip_proto, sizeof(ft->uf.ip_proto));
    ipfix_register_callback(t, _ftrace_semcb_v6, ft);

    /* FIXME more templates */

    /* then start the reader thread */
    pthread_create(ft->rt, NULL, _ftrace_rthread, ft);

    return &ft->uf;
}

/** Destroy a uniflow structure created by ftrace_create_uniflow */
void ftrace_destroy_uniflow(libftrace_uniflow_t *uf) {
    /* FIXME we need a way to signal the inner ipfix to stop reading */
}

/** Read the next uniflow from a libftrace reader. 
    Skips records in the stream which do not match uniflows. */
int ftrace_next_uniflow(libftrace_uniflow_t *uf) {

    /* signal write ready */
    pthread_cond_signal(&uf->_ft.wok);

    /* wait read ready */
    pthread_cond_wait(&uf->_ft.rok);

    /* check valid */
    return uf->_valid;
}