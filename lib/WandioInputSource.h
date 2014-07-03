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

#ifndef _LIBFC_WANDIOINPUTSOURCE_H_
#  define _LIBFC_WANDIOINPUTSOURCE_H_

#  include <string>

extern "C" {
#  include <wandio.h>
}

#include "InputSource.h"
#include "ErrorContext.h"

namespace libfc {

  class WandioInputSource : public InputSource {
  public:
    /** Creates a wandio input source from an io_t.
     *
     * @param io the io_t pointer belonging to a data file
     * @param name the name you want this file to be known to diagnostics
     */
    WandioInputSource(io_t* io, std::string name);

    /** Creates a wandio input source from a file name.
     *
     * @param name the file name
     */
    WandioInputSource(std::string name);

    ~WandioInputSource();

    ssize_t read(uint8_t* buf, uint16_t len);
    ssize_t peek(uint8_t* buf, uint16_t len);
    bool resync();
    size_t get_message_offset() const;
    void advance_message_offset();
    const char* get_name() const;
    bool can_peek() const;

      /** Returns the error generated when attempting to open the input source.
       *  After creating an InputSource that might fail to open, check this
       *  to determine whether the open succeeded, and to retrieve the error if not.
       *
       * @return a shared pointer to an error context, NULL on success
       */
      std::shared_ptr<ErrorContext> get_error() {
          return open_err;
      }

  private:
    io_t* io;
    size_t message_offset;
    size_t current_offset;
    std::string name;
    bool io_belongs_to_me;

  protected:
      // stupid to put this here, but InputSource and ErrorContext are circular.
      /** Stores an error context related
       to attempting to open the input source
       */
      std::shared_ptr<ErrorContext> open_err;
  };

} // namespace libfc

#endif // _LIBFC_WANDIOINPUTSOURCE_H_
