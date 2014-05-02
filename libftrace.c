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
    /** templates to match for setting ip_version field */
    struct {
        ipfix_template_t    *v4;
        ipfix_template_t    *v6;
    }
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

void _ftrace_semcb(const struct ipfix_template_t *t) {

}

void _ftrace_rthread(void *vpft) {
    libftrace_t *ft = (libftrace_t *)vpft;

    
}

/** Start reading uniflows from a libftrace source */
libftrace_uniflow_t *ftrace_start_uniflow(libftrace_t *ft)
{
    /* check to see if we've already registered a uniflow */
    if (ft->uf._ft == ft) return &ft->uf;

    /* register base template v4 */
    ft->tmpl.v4 = ipfix_template_new(ft->ts);
    ipfix_register_placement(ft->tmpl.v4, "flowStartMilliseconds", 
        &ft->uf.time_start, sizeof(ft->uf.time_start));
    ipfix_register_placement(ft->tmpl.v4, "flowEndMilliseconds", 
        &ft->uf.time_end, sizeof(ft->uf.time_end));
    ipfix_register_placement(ft->tmpl.v4, "packetDeltaCount", 
        &ft->uf.packets, sizeof(ft->uf.packets));
    ipfix_register_placement(ft->tmpl.v4, "octetDeltaCount", 
        &ft->uf.octets, sizeof(ft->uf.octets));
    ipfix_register_placement(ft->tmpl.v4, "sourceIPv4Address", 
        &ft->uf.ip.v4.src, sizeof(ft->uf.ip.v4.src));
    ipfix_register_placement(ft->tmpl.v4, "destinationIPv4Address", 
        &ft->uf.ip.v4.dst, sizeof(ft->uf.ip.v4.dst);
    ipfix_register_placement(ft->tmpl.v4, "sourceTransportPort",
        &ft->uf.port_src, sizeof(ft->uf.port_src));
    ipfix_register_placement(ft->tmpl.v4, "destinationTransportPort",
        &ft->uf.port_dst, sizeof(ft->uf.port_dst));
    ipfix_register_placement(ft->tmpl.v4, "protocolIdentifier",
        &ft->uf.ip_proto, sizeof(ft->uf.ip_proto));

    /* register base template v6 */
    ft->tmpl.v6 = ipfix_template_new(ft->ts);
    ipfix_register_placement(ft->tmpl.v6, "flowStartMilliseconds", 
        &ft->uf.time_start, sizeof(ft->uf.time_start));
    ipfix_register_placement(ft->tmpl.v6, "flowEndMilliseconds", 
        &ft->uf.time_end, sizeof(ft->uf.time_end));
    ipfix_register_placement(ft->tmpl.v6, "packetDeltaCount", 
        &ft->uf.packets, sizeof(ft->uf.packets));
    ipfix_register_placement(ft->tmpl.v6, "octetDeltaCount", 
        &ft->uf.octets, sizeof(ft->uf.octets));
    ipfix_register_placement(ft->tmpl.v6, "sourceIPv6Address", 
        &ft->uf.ip.v6.src, sizeof(ft->uf.ip.v6.src));
    ipfix_register_placement(ft->tmpl.v6, "destinationIPv6Address", 
        &ft->uf.ip.v6.dst, sizeof(ft->uf.ip.v6.dst);
    ipfix_register_placement(ft->tmpl.v6, "sourceTransportPort",
        &ft->uf.port_src, sizeof(ft->uf.port_src));
    ipfix_register_placement(ft->tmpl.v6, "destinationTransportPort",
        &ft->uf.port_dst, sizeof(ft->uf.port_dst));
    ipfix_register_placement(ft->tmpl.v6, "protocolIdentifier",
        &ft->uf.ip_proto, sizeof(ft->uf.ip_proto));

    /* FIXME more templates */

    /* bind a callback to the set to handle semaphores */
    ipfix_register_callback(ft->ts, _ftrace_semcb);

    /* then start the reader thread */
    pthread_create(ft->rt, NULL, _ftrace_rthread, ft);
}

/** Destroy a uniflow structure created by ftrace_create_uniflow */
void ftrace_destroy_uniflow(libftrace_uniflow_t *uf) {

}

/** Read the next uniflow from a libftrace reader. 
    Skips records in the stream which do not match uniflows. */
int ftrace_read_uniflow(libftrace_uniflow_t *uf) {

    /* signal write ready */
    pthread_cond_signal(&uf->_ft_wok);

    /* wait read ready */
    pthread_cond_wait(&uf->_ft.rok);

    /* FIXME check EOF */

    /* return code */
}