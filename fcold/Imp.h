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
 * @author Brian Trammell <trammell@tik.ee.ethz.ch>
 */

#ifndef _FCOLD_IMP_H_
#  define _FCOLD_IMP_H_

#include <condition_variable>
#include <queue>
#include <thread>    

#include "ErrorContext.h"
#include "MessageBuffer.h"
#include "PlacementCollector.h"
#include "ReaderWriterQueue.h"

namespace fcold {

    class Imp : public libfc::PlacementCollector {

    private:
        std::thread                     worker;
        std::queue<std::shared_ptr<MessageBuffer> >
                                        mbq;
        std::mutex                      mbqmtx;
        std::shared_ptr<libfc::ErrorContext>   worker_ectx;
        std::condition_variable         mbqcv;
        bool                            run;

        std::shared_ptr<MessageBuffer>  next_mbuf();
        void                            work();
    
    protected:
        virtual std::shared_ptr<libfc::ErrorContext>
                start_message(std::shared_ptr<MessageBuffer> msg);
        
    public:
        Imp(libfc::PlacementCollector::Protocol protocol);
        virtual ~Imp();
        
        void enqueue_mbuf(std::shared_ptr<MessageBuffer> mbuf);
        void start();
        void stop();
    };
} /* namespace fcold */

#endif /* defined(_FCOLD_IMP_H_) */
