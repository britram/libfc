/* Hi Emacs, please use -*- mode: C++; -*- */
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
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR 
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL ETH 
 * ZURICH BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
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

#ifndef _LIBFC_ERRORHANDLER_H_
#  define _LIBFC_ERRORHANDLER_H_

#  include "Error.h"

namespace IPFIX {

  class ErrorHandler {
  public:
    /** An error has occurred during the parsing of a message.
     *
     * This means that the message must be abandoned because it cannot
     * be recovered, but that an attempt may be made to read more
     * messages from the message stream.  For file- or TCP-based input
     * streams this could mean reading byte by byte until a
     * plausible-looking message header is encountered again.  For
     * UDP-based input streams, this may mean simply discarding the
     * current message and receiving the next one.
     */
    virtual void error(Error error, const char* message) = 0;

    /** A fatal error has occurred during the parsing of a message.
     *
     * This means that the entire message stream must be abandoned
     * because it cannot be recovered, for example due to a read
     * error.
     */
    virtual void fatal(Error error, const char* message) = 0;

    /** A strange circumstance has occurred during the parsing of a
     * message.
     *
     * This means that the message can be recovered, but somethign
     * strange has happened that wouldn't be expected during normal
     * operations.
     */
    virtual void warning(Error error, const char* message) = 0;
  };



} // namespace IPFIX

#endif // _LIBFC_ERRORHANDLER_H_
