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
#include <unistd.h>
#include <errno.h>

#if defined(_LIBFC_HAVE_LOG4CPLUS_)
#  include <log4cplus/loggingmacros.h>
#else
#  define LOG4CPLUS_TRACE(logger, expr)
#endif /* defined(_LIBFC_HAVE_LOG4CPLUS_) */

#include "Constants.h"
#include "FileExportDestination.h"

namespace libfc {

  FileExportDestination::FileExportDestination(int _fd)
    : fd(_fd)
#if defined(_LIBFC_HAVE_LOG4CPLUS_)
    , logger(log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("FileExportDestination")))
#endif /* defined(_LIBFC_HAVE_LOG4CPLUS_) */
 {
  }

  ssize_t FileExportDestination::writev(const std::vector< ::iovec>& iovecs) {
    LOG4CPLUS_TRACE(logger, "ENTER FileExportDestination::writev");
    LOG4CPLUS_TRACE(logger, "writing " << iovecs.size() << " iovecs");
#if defined(_LIBFC_HAVE_LOG4CPLUS_)
    const ::iovec* vecs = iovecs.data();
    size_t total = 0;
    for (unsigned int i = 0; i < iovecs.size(); i++) {
      LOG4CPLUS_TRACE(logger, "  iovec[" << i << "]"
                      << "@" << vecs[i].iov_base
                      << "[" << vecs[i].iov_len << "]");
      total += vecs[i].iov_len;
    }
    LOG4CPLUS_TRACE(logger, "total=" << total);
#endif /*  defined(_LIBFC_HAVE_LOG4CPLUS_) */

    return ::writev(fd, iovecs.data(), static_cast<int>(iovecs.size()));
  }

  int FileExportDestination::flush() {
    return 0;
  }

  bool FileExportDestination::is_connectionless() const {
    return false;
  }

  size_t FileExportDestination::preferred_maximum_message_size() const {
    return kMaxMessageLen;
  }



} // namespace libfc
