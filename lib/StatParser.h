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
#ifndef _LIBFC_STATPARSER_H_
#  define _LIBFC_STATPARSER_H_

#  include <cstdint>
#  include <fstream>
#  include <string>

#  if defined(_LIBFC_HAVE_LOG4CPLUS_)
#    include <log4cplus/logger.h>
#  endif /* defined(_LIBFC_HAVE_LOG4CPLUS_) */

namespace libfc {

  /** Parses a stat file.
   *
   * Stat files give insight into the structure of their corresponding
   * dat files (containing the flow records).  Knowing what comes next
   * can make processing dat files much easier.  This class parses
   * such stat files.
   */
  class StatParser {
  public:
    /** The parser state.
     *
     * Inquire about this with get_state() when you call next_p() and
     * get an error.  Inquire about an error message with
     * get_message(). */
    enum state_t {
      /** Initial state. */
      init,
      
      /** Everything OK so far. */
      good_state,
      
      /** File ended, everything OK. */
      eof_state,
      
      /** File is syntactically corrupt. */
      parse_error,
      
      /** There was a read error. */
      io_error,

      /** There was some other error. */
      other_error,
    };

    /** Creates a StatParser that parses a given file.
     *
     * You should check by calling good() after the constructor to see
     * if the parser could be initialised correctly.
     *
     * @param filename the file to parse
     */
    StatParser(const std::string filename);

    /** Gets the next P-line from the stat file.
     *
     * @param length pointer to the message length
     * @param source_address pointer to an array of at least four
     *   uint8_t's; will contain router's IP address in network byte
     *   order.
     * @param port the router's port
     *
     * @return true if parsing went OK and the parameters should now
     * point to good values; false if there was an error of some
     * kind.  In this case, check with get_state() and get_message().
     *
     * @warning If this method returns false, you must not assume that
     * the previous contents of the parameters have been preserved.
     * In fact, the contents of those parameters must be considered
     * undefined.
     */
    bool next_p(uint16_t* length, uint8_t* source_address, uint16_t* port);
    
    /** Checks if the state of this parser is still good.
     *
     * This is a convenience function, equivalent to get_state() ==
     * good_state.
     *
     * @return true if the state is good, false otherwise.
     */
    bool good() const;

    /** Checks if parsing terminated due to EOF.
     *
     * This is a convenience function, equivalent to get_state() ==
     * eof_state.
     *
     * @return true if the file is at EOF, false otherwise.
     */
    bool eof() const;

    /** Returns the state of this parser.
     *
     * @return the state of this parser
     */
    state_t get_state() const;

    /** Returns the most recent error message. 
     *
     * @return the most recent error message
     */
    const std::string get_message() const;

  private:
    std::string filename;
    std::ifstream is;

    unsigned int line_no;
    state_t state;
    std::string message;

#  if defined(_LIBFC_HAVE_LOG4CPLUS_)
    log4cplus::Logger logger;
#  endif /* defined(_LIBFC_HAVE_LOG4CPLUS_) */

    bool parse_uint8(std::string line, unsigned int* i,
                     uint8_t* val, char delim, 
                     const std::string what);

    bool parse_uint16(std::string line, unsigned int* i, uint16_t* val,
                      const std::string what);
  };

} // namespace libfc

#endif /* _LIBFC_STATPARSER_H_ */
