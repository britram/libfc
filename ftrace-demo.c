/* Copyright (c) 2011-2014 ETH Zürich. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *    * Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *    * Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *    * The name of ETH Zürich nor the names of other contributors
 *      may be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL ETH
 * ZURICH BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
 * IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE
 */

/**
 *
 * Demonstrate the usage of the libftrace library.
 *
 *
 * @author Brian Trammell <trammell@tik.ee.ethz.ch>
 *
 */

#include <stdio.h>
#include <getopt.h>

#include "libftrace.h"

#include <tomcrypt.h>

static const char* spec_file_name = 0;
static int verbose_flag = false;
static int help_flag = false;
static int message_version = 10;
static const char* filename = 0;

/* Code patterned after http://www.gnu.org/software/libc/
 * manual/html_node/Getopt-Long-Option-Example.html
 * #Getopt-Long-Option-Example */
static void parse_options(int argc, char* const* argv) {

    static struct option options[] = {
        { "help", no_argument, &help_flag, 1 },
        { "input", required_argument, 0, 'i' },
        { "verbose", no_argument, &verbose_flag, 1 },
        { "message-version", required_argument, 0, 'm' },
        { "specfile", required_argument, 0, 's' },
        { 0, 0, 0, 0 },
    };
    
    
    while (1) {

        int option_index = 0;
        int c = getopt_long(argc, argv, "hi:m:s:v", options, &option_index);
        if (c == -1)
            break;
        
        switch(c) {
            case 0:
                break;
            case 'i':
                filename = optarg;
                break;
            case 'm':
                message_version = atoi(optarg);
                if (message_version != 9 && message_version != 10) {
                    fprintf("Message version %s not supported\n", optarg);
                    exit(EXIT_FAILURE);
                }
                break;
            case 's':
                spec_file_name = optarg;
                break;
            default:
                fprintf(stderr, "Unrecognised option character %c, terminating.\n", c);
                exit(EXIT_FAILURE);
        }
    }
}

static uint64_t hash_addr4(int addr4) {
    
    /* FIXME need a way to pass this in */
    uint8_t salt[8] = { 0x01, 0xc0, 0xff, 0xee, 0x07, 0xea, 0x12, 0x19 };
    uint64_t sha_out[4];
    
    hash_state      sha;
    
    sha256_init(&sha);
    sha256_process(&sha, salt, 8);
    sha256_process(&sha, &addr4, sizeof(addr4));
    sha256_done(&sha, &sha_out);
    return sha_out[0];
}

static uint64_t hash_addr6(uint8_t *addr6) {
    
    /* FIXME need a way to pass this in */
    uint8_t salt[8] = { 0x01, 0xc0, 0xff, 0xee, 0x07, 0xea, 0x12, 0x19 };
    uint64_t sha_out[4];
    
    hash_state      sha;
    
    sha256_init(&sha);
    sha256_process(&sha, salt, 8);
    sha256_process(&sha, addr6, 16);
    sha256_done(&sha, (uint8_t *)&sha_out);
    return sha_out[0];
}

int main (int argc, char *argv[]) {
    
    libftrace_t             *ft;
    libftrace_uniflow_t     *uf;
    
    uint64_t                srcid, dstid;
    uint64_t                reltime, basetime = 0;
    
    int rv;
    
    parse_options(argc, argv);
    
    ft = ftrace_create(filename, message_version);

    if (spec_file_name) (void)ftrace_add_specfile(ft, spec_file_name);
    
    uf = ftrace_start_uniflow(ft);
    
    while ((rv = ftrace_next_uniflow(uf)) >= 1) {
        
        /* short-circuit: ensure it's a real port 443 TCP/IPv4 
           flow with at least three packets */
        if (uf->ip_ver != 4 ||
            uf->ip_proto != 6 ||
            uf->port_src != 443 ||
            uf->packets < 3) {
            continue;
        }
        
        /* Hash the addresses */
        srcid = hash_addr4(uf->ip.v4.src);
        dstid = hash_addr4(uf->ip.v4.dst);
        
        /* Offset the time */
        if (!basetime) {
            basetime = uf->time_start - 3600000;
        }
        
        reltime = uf->time_start - basetime;
        
        fprintf(stdout, "%10.3f, %016llx, %016llx\n", (float)reltime / 1000.0, srcid, dstid);
    }
    
    if (rv < 0) {
        fprintf(stderr, "there was an error but i don't know what it was...\n");
        return -1;
    }
    
    ftrace_destroy_uniflow(uf);
    ftrace_destroy(ft);
}

