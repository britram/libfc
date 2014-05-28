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
#include <cassert>
#include <cerrno>
#include <cstdlib>
#include <iostream>

#include <sys/types.h>

#if defined (__linux__)
#  include <linux/limits.h>
#  include <sys/inotify.h>
#endif /* defined (__linux__) */

#include "DirectoryListener.h"

namespace fcold {

  DirectoryListener::DirectoryListener(const std::string& directory_name) 
#if defined (__linux__)
    : listener_fd(-1),
      watching_fd(-1)
#endif /* defined (__linux__) */
    {

    errno = 0;

#if defined (__linux__)
    listener_fd = ::inotify_init();
    if (listener_fd < 0) {
      system_errno = errno;
      return;
    }

    watching_fd = inotify_add_watch(listener_fd, directory_name.c_str(),
                                    IN_CREATE);
    if (watching_fd < 0) {
      system_errno = errno;
      return;
    }
#endif /* defined (__linux__) */

    good = true;
  }

  DirectoryListener::~DirectoryListener() {
#if defined (__linux__)
    if (is_good()) {
      if (inotify_rm_watch(listener_fd, watching_fd) < 0)
        ; // Ignore for now;
      if (close(listener_fd) < 0)
        ; // Ignore for now
    }
#endif /* defined (__linux__) */
  }

  void DirectoryListener::listen() {
    if (is_good()) {
#if defined (__linux__)
      /* I don't like this, but this is how it's done; see `man inotify(7)'. */
      uint8_t buf[sizeof(struct inotify_event) + PATH_MAX];
      struct inotify_event *current_event 
        = reinterpret_cast<struct inotify_event*>(&buf[0]);

      while (listening) {
        errno = 0;
        ssize_t nbytes = read(watching_fd, buf, sizeof buf);
        if (nbytes < 0) {
          if (errno == EAGAIN || errno == EINTR)
            continue;

          good = false;
          system_errno = errno; // EINVAL == buffer too short
          break;
        }

        /* Assume no short reads */
        assert(nbytes > 0 // To make static_cast safe
               && static_cast<size_t>(nbytes) >= sizeof(struct inotify_event));

        /* We watch only fir file (not directory) creation. */
        assert((current_event->mask & IN_CREATE)
               && !(current_event->mask & IN_ISDIR));

        /* This is a null-terminated string. */
        std::string filename{current_event->name};

        /* Now do something with filename */
        std::cout << "Created " << filename << std::endl;
      }
#endif /* defined (__linux__) */
    }
  }

} // namespace fcold

