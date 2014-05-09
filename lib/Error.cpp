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
#include <cassert>

#include "Error.h"

namespace IPFIX {

  Error::Error(error_t e)
    : e(e) {
  }

  const std::string Error::to_string() const {
    switch (e) {
    case no_error: return "no error"; break;
    case parse_while_parsing: return "call to parse() while parsing"; break;
    case system_error: return "system error"; break;
    case short_header: return "short message header"; break;
    case short_body: return "short message body"; break;
    case long_set: return "set too long (exceeds message size)"; break;
    case long_fieldspec: return "field specification exceeds set"; break;
    case message_version_number: return "unexpected version number"; break;
    case short_message: return "short message"; break;
    case ipfix_basetime: return "got basetime in IPFIX message"; break;
    case format_error: return "format error"; break;
    }
    // This fall-through cannot happen if the switch above contains
    // all enum values and the object has been initialised properly.
    // Still, gcc complains that "control reaches end of non-void
    // function", so this is to shut the compiler up. --neuhaust
    assert(false);
    return "unknown error";
 }

} // namespace IPFIX
