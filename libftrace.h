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

/**
 * @file libftrace.h
 * @brief Libtrace-like interface to IPFIX uniflow records
 * @author Brian Trammell <trammell@tik.ee.ethz.ch>
 *
 */

#ifndef _LIBFTRACE_H_ /* idem */
#define _LIBFTRACE_H_ /* hack */

/** Opaque structure representing a libftrace source */
struct libftrace_st;
typedef struct libftrace_st libftrace_t;

/** Structure representing a unidirectional flow */
typedef struct libftrace_uniflow_st {
  /** Parent libftrace source structure */
  libftrace_t     *_ft;
  /** Record valid flag */
  int             _valid;
  /** Flow start time in POSIX epoch milliseconds */
  uint64_t        time_start;
  /** Flow end time in POSIX epoch milliseconds */
  uint64_t        time_end;
  /** Packet count */
  uint64_t        packets;
  /** Octet count */
  uint64_t        octets;
  union {
    struct {
      /** IPv4 source address */
      uint32_t    src;
      /** IPv4 destination address */
      uint32_t    dst;
      /** IPv4 next-hop address */
      uint32_t    nexthop;
    } v4;
    struct {
      /** IPv6 source address (network byte order) */
      uint8_t     src[16];
      /** IPv6 destination address (network byte order) */
      uint8_t     dst[16];
      /** IPv6 next-hop address (network byte order) */
      uint8_t     nexthop[16];
    } v6;
  } ip;
  /** Input interface identifier. Only has meaning within the scope of the exporter. */
  uint32_t        iface_in;
  /** Output interface identifier. Only has meaning within the scope of the exporter. */
  uint32_t        iface_out;
  /** Previous or source BGP AS number, if known; otherwise 0. */
  uint32_t        bgp_as_in;
  /** Next or destination BGP AS number, if known; otherwise 0. */
  uint32_t        bgp_as_out;
  /** Source TCP or UDP port; undefined if not TCP or UDP. */
  uint16_t        port_src;
  /** Destination TCP or UDP port; undefined if not TCP or UDP. */
  uint16_t        port_dst;
  /** IP version (4 or 6) */
  uint8_t         ip_ver;
  /** IP protocol identifier */
  uint8_t         ip_proto;
  /** Content of the TOS byte on the first packet of the flow */
  uint8_t         ip_tos;
  /** Union of all TCP flags seen over the flow; undefined if not TCP */
  uint8_t         tcp_flags;
} libftrace_uniflow_t;

/** Open a libftrace reader on an IPFIX/PDU file */
libftrace_t *ftrace_create(const char *filename);

/** Close a libftrace reader */
void ftrace_destroy(libftrace_t *ft);

/** Create a libftrace uniflow structure for reading uniflows */
libftrace_uniflow_t *ftrace_start_uniflow(libftrace_t *ft);

/** Stop reading from a uniflow context *//
void ftrace_destroy_uniflow(libftrace_uniflow_t *uf);

/** Read the next uniflow from a libftrace reader. 
    Skips records in the stream which do not match uniflows. */
int ftrace_next_uniflow(libftrace_t *ft, libftrace_uniflow_t *uf);


#endif /* idem hack */
