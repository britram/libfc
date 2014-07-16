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


#if defined (__linux__)
#  include <linux/limits.h>
#  include <sys/inotify.h>
#  include <sys/select.h>
#endif /* defined (__linux__) */

#include "DirectoryListener.h"

namespace fcold {

  DirectoryListener::DirectoryListener(ImpFactory& impfact,
                     const std::string& directory_name):
    Listener(impfact)
#if defined (__linux__)
                             ,
      listener_fd(-1),
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
                                    IN_CLOSE_WRITE);
    if (watching_fd < 0) {
      system_errno = errno;
      return;
    }
#endif /* defined (__linux__) */

    good = true;
  }

  DirectoryListener::~DirectoryListener() {
#if defined (__linux__)
    if (good) {
      if (watching_fd >= 0 && inotify_rm_watch(listener_fd, watching_fd) < 0)
        ; // Ignore for now;
      if (listener_fd >= 0 && close(listener_fd) < 0)
        ; // Ignore for now
    }
#endif /* defined (__linux__) */
  }

  void DirectoryListener::listen() {
#if defined (__linux__)
    /* I don't like this, but this is how it's done; see `man inotify(7)'
     * and `man 7 pthread'. 
     *
     * First, the inotify API needs to transport variable-length
     * information, and they solve it by having that as the last
     * element of the struct with zero length.  Nice. (Hence the
     * weird-looking size for BUF below.
     *
     * And then there seems to be no clean way to break a thread out
     * of a blocking system call.  Apparently, something can and
     * will always go wrong. Memory leaks are just one among the
     * many horrors of interrupting threads.  So i'm closing the file
     * descriptor on which this thread will be blocking. Ugh!
     */
    uint8_t buf[sizeof(struct inotify_event) + PATH_MAX];
    struct inotify_event *current_event 
      = reinterpret_cast<struct inotify_event*>(&buf[0]);
    
    while (good && listening) {
      errno = 0;
      ssize_t nbytes = read(listener_fd, buf, sizeof buf);
      if (nbytes < 0) {
        if (errno == EAGAIN)
          continue;
        else if (errno == EINTR)
          break;
        else {
          good = false;
          system_errno = errno; // EINVAL == buffer too short
          break;
        }
      }
      
      /* Assume no short reads */
      assert(nbytes > 0 // To make static_cast safe
             && static_cast<size_t>(nbytes) >= sizeof(struct inotify_event));
      
       /* No idea what IN_IGNORED means, but it's what happens when
        * you close the watching_fd under this thread's nose. */
      if (current_event->mask & IN_IGNORED)
        break;

      /* IN_UNMOUNT happens when the filesystem in question is
       * unmounted, and IN_Q_OVERFLOW means there were too many
       * unfetched events, and in both cases there's nothing more to
       * do here. */
      if (current_event->mask & IN_Q_OVERFLOW
          || current_event->mask & IN_UNMOUNT)
        /* TODO: warn here */
        break;

      assert(current_event->mask & IN_CLOSE_WRITE);

      /* This is a null-terminated string. */
      std::string filename{current_event->name};
      
      /* TODO: now do something with filename */
    }
#endif /* defined (__linux__) */
  }

  void DirectoryListener::stop() {
    listening = false;
#if defined (__linux__)
    errno = 0;
    if (watching_fd >= 0 && inotify_rm_watch(listener_fd, watching_fd) < 0) {
      good = false;
      system_errno = errno;
    }
#endif /* defined (__linux__) */
    listener.join();
  }


} // namespace fcold

