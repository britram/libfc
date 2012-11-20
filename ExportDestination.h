/* Hi Emacs, please use -*- mode: C++; -*- */
/* Copyright (c) 2011-2012 ETH Zürich. All rights reserved.
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
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR 
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT 
 * HOLDERBE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY 
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file
 * @author Stephan Neuhaus <neuhaust@tik.ee.ethz.ch>
 */

#ifndef IPFIX_EXPORTDESTINATION_H
#  define IPFIX_EXPORTDESTINATION_H

#  include <cstdint>
#  include <vector>

/* For struct iovec */
#  include <sys/uio.h>

/* For ssize_t and size_t */
#  include <unistd.h>

namespace IPFIX {

  /** Abstract base class for IPFIX outputs.
   *
   * This class defines the interface that all IPFIX outputs
   * must implement.
   */
  class ExportDestination {
  public:
    /** Writes a set of scattered buffers.
     *
     * @param iovecs a vector of struct iovec (see `man writev')
     * @return 0 on success, or -1 on error
     */
    virtual int writev(const std::vector< ::iovec>& iovecs) = 0;

    /** Checks whether this output is connection-oriented or not.
     *
     * This information is needed by the exporter: on connectionless
     * outputs, new messages must contain all the wire templates that
     * have accumulated in the meantime (or at least all wire
     * templates that are relevant for the message to come). This is
     * because it cannot be guaranteed that the receiver receives
     * messages in order and thus might miss messages containing
     * important wire templates otherwise.
     *
     * @return true if this output is connectionless, false if it
     *     is connection-oriented
     */
    virtual bool is_connectionless() const = 0;
  };

} // namespace IPFIX

#endif // IPFIX_EXPORTDESTINATION_H
