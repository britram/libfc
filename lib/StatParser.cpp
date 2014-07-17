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
#include <sstream>

#ifdef _LIBFC_HAVE_LOG4CPLUS_
#include <log4cplus/loggingmacros.h>
#else
#define LOG4CPLUS_TRACE(logger, expr)
#define LOG4CPLUS_WARN(logger, expr)
#define LOG4CPLUS_INFO(logger, expr)
#endif /* _LIBFC_HAVE_LOG4CPLUS_ */

#include "StatParser.h"

namespace libfc {

StatParser::StatParser(const std::string filename)
    : filename(filename), is(filename), line_no(0), state(init)
#ifdef _LIBFC_HAVE_LOG4CPLUS_
      ,
      logger(log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("StatParser")))
#endif /* _LIBFC_HAVE_LOG4CPLUS_ */
{
  if (is.good())
    state = good_state;
  else if (is.fail() || is.bad()) {
    state = io_error;
    message = "File \"" + filename + "\" couldn't be opened";
  } else {
    state = other_error;
    message = "Input file \"" + filename + "\"'s state isn't good() but "
                                           "neither is it fail() or bad()";
  }
}

static int char_to_int(char c) {
  switch (c) {
  case '0':
    return 0;
  case '1':
    return 1;
  case '2':
    return 2;
  case '3':
    return 3;
  case '4':
    return 4;
  case '5':
    return 5;
  case '6':
    return 6;
  case '7':
    return 7;
  case '8':
    return 8;
  case '9':
    return 9;
  default:
    return -1;
  }
}

#define MAKE_MESSAGE(m, expr)                                                  \
  do {                                                                         \
    std::stringstream is;                                                      \
    is << expr;                                                                \
    m = is.str();                                                              \
  } while (0)

bool StatParser::parse_uint16(std::string line, unsigned int *i, uint16_t *val,
                              const std::string what) {
  *val = 0;

  while (*i < line.length() && isdigit(line[*i])) {
    int next_digit = char_to_int(line[*i]);

    if (next_digit == -1) {
      state = parse_error;
      MAKE_MESSAGE(message,
                   filename << ":" << line_no << ": while parsing " << what
                            << ", supposed digit " << line[*i]
                            << " neither 0, 1, 2, 3, 4, 5, 6, 7, "
                               "8, or 9. Have we switched from decimal "
                               "to something else while I wasn't looking?");
      return false;
    }

    assert(next_digit >= 0 && next_digit <= 9);

    if (*val > (UINT16_MAX - next_digit) / 10) {
      state = parse_error;
      MAKE_MESSAGE(message, filename << ":" << line_no << ": " << what
                                     << " would overflow a uint16_t");
      return false;
    }

    *val = 10 * (*val) + next_digit;
    (*i)++;
  }
  return true;
}

bool StatParser::parse_uint8(std::string line, unsigned int *i, uint8_t *val,
                             char delim, const std::string what) {
  *val = 0;

  while (*i < line.length() && isdigit(line[*i])) {
    int next_digit = char_to_int(line[*i]);

    if (next_digit == -1) {
      state = parse_error;
      MAKE_MESSAGE(message,
                   filename << ":" << line_no << ": wile parsing " << what
                            << ", supposed digit " << line[*i]
                            << " neither 0, 1, 2, 3, 4, 5, 6, "
                               "7, 8, or 9. Have we switched from decimal "
                               "to something else while I wasn't looking?");
      return false;
    }

    assert(next_digit >= 0 && next_digit <= 9);

    if (*val > (UINT8_MAX - next_digit) / 10) {
      state = parse_error;
      MAKE_MESSAGE(message, filename << ":" << line_no << ": " << what
                                     << " would overflow a uint8_t");
      return false;
    }

    *val = 10 * (*val) + next_digit;
    (*i)++;
  }

  if (*i >= line.length()) {
    state = parse_error;
    MAKE_MESSAGE(message, filename << ":" << line_no << ": Line ends after "
                                   << what);
    return false;
  }

  if (line[*i] != delim) {
    state = parse_error;
    MAKE_MESSAGE(message, filename << ":" << line_no << ": Expected '" << delim
                                   << "' "
                                      "after " << what << ", but got '"
                                   << line[*i] << "'");
    return false;
  }
  (*i)++;
  return true;
}

bool StatParser::next_p(uint16_t *length, uint8_t *source_address,
                        uint16_t *port) {
  if (state != good_state)
    return false;

  bool found = false;

  while (!found) {
    std::string line;
    if (!std::getline(is, line)) {
      if (is.eof()) {
        state = eof_state;
        MAKE_MESSAGE(message, "Parse of \""
                                  << filename
                                  << "\" terminated normally due to EOF");
      } else if (is.fail() || is.bad()) {
        state = io_error;
        MAKE_MESSAGE(message, "Parse of \"" << filename
                                            << "\" terminated abnormally "
                                               "due to an I/O error");
      } else {
        state = other_error;
        MAKE_MESSAGE(message, "Parse of \"" << filename
                                            << "\" terminated abnormally "
                                               "due to an unknown error");
      }
      return false;
    }

    line_no++;
    if (line.length() > 0 && line[0] == 'P') {
      found = true;
      unsigned int i = 1;
      while (i < line.length() && isspace(line[i]))
        i++;

      if (i >= line.length()) {
        state = parse_error;
        MAKE_MESSAGE(message, filename << ":" << line_no
                                       << ": Line ends after P");
        return false;
      }

      if (!isdigit(line[i])) {
        state = parse_error;
        MAKE_MESSAGE(message, filename << ":" << line_no
                                       << ": Expected digits after P "
                                          "and spaces, but got '" << line[i]
                                       << "'");
        return false;
      }

      while (i < line.length() && isdigit(line[i]))
        i++;

      if (i >= line.length()) {
        state = parse_error;
        MAKE_MESSAGE(message, filename << ":" << line_no
                                       << ": Line ends after packet number");
        return false;
      }

      if (!isspace(line[i])) {
        state = parse_error;
        MAKE_MESSAGE(message, filename << ":" << line_no
                                       << ": Expected spaces after "
                                          "packet number, but got '" << line[i]
                                       << "'");
        return false;
      }

      while (i < line.length() && isspace(line[i]))
        i++;

      if (i >= line.length()) {
        state = parse_error;
        MAKE_MESSAGE(message, filename << ":" << line_no
                                       << ": Line ends after packet "
                                          "number and spaces");
        return false;
      }

      if (!isdigit(line[i])) {
        state = parse_error;
        MAKE_MESSAGE(message, filename << ":" << line_no
                                       << ": Expected digits after "
                                          "packet number, but got '" << line[i]
                                       << "'");
        return false;
      }

      while (i < line.length() && isdigit(line[i]))
        i++;

      if (i >= line.length()) {
        state = parse_error;
        MAKE_MESSAGE(message, filename << ":" << line_no
                                       << ": Line ends after integer "
                                          "part of relative time");
        return false;
      }

      if (line[i] != '.') {
        state = parse_error;
        MAKE_MESSAGE(message,
                     filename << ":" << line_no
                              << ": Relative time seems not to be a "
                                 "floating-point number (no decimal point)");
        return false;
      }

      i++;
      while (i < line.length() && isdigit(line[i]))
        i++;

      if (i >= line.length()) {
        state = parse_error;
        MAKE_MESSAGE(message, filename << ":" << line_no
                                       << ": Line ends after relative time");
        return false;
      }

      if (!isspace(line[i])) {
        state = parse_error;
        MAKE_MESSAGE(message, filename << ":" << line_no
                                       << ": Expected spaces after "
                                          "relative time, but got '" << line[i]
                                       << "'");
        return false;
      }

      while (i < line.length() && isspace(line[i]))
        i++;

      if (i >= line.length()) {
        state = parse_error;
        MAKE_MESSAGE(message,
                     filename << ":" << line_no
                              << ": Line ends after relative time and spaces");
        return false;
      }

      if (!isdigit(line[i])) {
        state = parse_error;
        MAKE_MESSAGE(message, filename << ":" << line_no
                                       << ": Expected digits after "
                                          "relative time, but got '" << line[i]
                                       << "'");
        return false;
      }

      if (!parse_uint16(line, &i, length, "length"))
        return false;

      if (i >= line.length()) {
        state = parse_error;
        MAKE_MESSAGE(message, filename << ":" << line_no
                                       << ": Line ends after length");
        return false;
      }

      if (!isspace(line[i])) {
        state = parse_error;
        MAKE_MESSAGE(message,
                     filename << ":" << line_no
                              << ": Expected spaces after length, but got '"
                              << line[i] << "'");
        return false;
      }

      while (i < line.length() && isspace(line[i]))
        i++;

      if (i >= line.length()) {
        state = parse_error;
        MAKE_MESSAGE(message,
                     filename << ":" << line_no
                              << ": Line ends after length and spaces");
        return false;
      }

      if (!isdigit(line[i])) {
        state = parse_error;
        MAKE_MESSAGE(message,
                     filename << ":" << line_no
                              << ": Expected digits after length, but got '"
                              << line[i] << "'");
        return false;
      }

      if (!parse_uint8(line, &i, &source_address[0], '.',
                       "first IP address octet"))
        return false;
      if (!parse_uint8(line, &i, &source_address[1], '.',
                       "second IP address octet"))
        return false;
      if (!parse_uint8(line, &i, &source_address[2], '.',
                       "third IP address octet"))
        return false;
      if (!parse_uint8(line, &i, &source_address[3], ':',
                       "fourth IP address octet"))
        return false;
      if (!parse_uint16(line, &i, port, "port"))
        return false;

      while (i < line.length() && isspace(line[i]))
        i++;

      if (i < line.length()) {
        state = parse_error;
        MAKE_MESSAGE(
            message,
            filename << ":" << line_no
                     << ": There is non-blank junk at the end of the line");
        return false;
      }

      return true;
    }
  }

  assert("You should never reach this" == 0);
  return true;
}

bool StatParser::good() const { return state == good_state; }

bool StatParser::eof() const { return state == eof_state; }

StatParser::state_t StatParser::get_state() const { return state; }

const std::string StatParser::get_message() const { return message; }

} // namespace libfc
