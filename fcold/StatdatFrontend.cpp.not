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

namespace fcold {


    static std::string filename_to_stat(const std::String& filename) {
        auto datidx = filename.rfind(".dat", filename.size() - 1);
        if (datidx == std::string::npos)
            // This will probably never work, but hey at least we tried.
            return filename + ".stat";
        else
            return filename.substr(0, datidx) + ".stat" +
                   filename.substr(datidx + 4, filename.size() - (datidx + 4));
    }
    
    StatdatFrontend::next_statline()
    {
        // fixme do stuff
    }
    
    StatdatFrontend::StatdatFrontend(ImpFactory&        impfact,
                                     const std::string& filename):
        WandioFrontend(impfact, filename)
    {
        // Attempt to open the associated statfile
        stat_filename = filename_to_stat(filename);
        stat_is = new WandioInputSource(stat_filename);
        if (stat_is->get_error()) {
            // FIXME report error: can't open statfile
            delete stat_is;
            stat_is = nullptr;
        }
        
    }
    
    std::shared_ptr<libfc::ErrorContext>
        StatdatFrontend::deframe_next(std::shared_ptr<MessageBuffer>& mb)
    {
        // get the message first
        WandioFrontend::deframe_next(mb);
        
        // now get the next stat line if we can
        if (stat_is) next_statline();
    }
}