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
#ifndef _FCOLD_LISTENER_H_
#  define _FCOLD_LISTENER_H_

#  include <thread>

namespace fcold {

  class Listener {
  public:
    /** Creates a Listener. */
    Listener();

    /** Destroys a Listener. */
    virtual ~Listener() = 0;

    /** Listens for an event.
     *
     * This member function blocks until an event that this object
     * listens for occurs, and then acts appropriately.
     */
    virtual void listen() = 0;

    /** Returns the state of this listener.
     *
     * Listeners are created through their constructors, but things
     * may go wrong there.  The is_good() member function should be
     * used after creating a Listener to see if everything went OK.
     * if is_good() returns false, a further call to get_errno() may
     * (or may not) reveal the reason.
     */
    bool is_good() const;

    /** Returns the system error that led to is_good() returning false.
     *
     * When is_good() returns false, a call to get_errno() reveals the
     * system's errno at the first failing call in the constructor.
     * This may be zero if the reason for the constructor failure is
     * not a failed system or system library call.
     */
    int get_errno() const;

    /** Tells the listener to stop listening.
     *
     * This will interrupt the thread that is currently running
     * listen().
     */
    virtual void stop() = 0;

  protected:
    bool good;
    int system_errno;
    bool listening;
    std::thread listener;
  };

} // namespace fcold

#endif /* _FCOLD_LISTENER_H_ */
