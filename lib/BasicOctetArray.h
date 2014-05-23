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
#ifndef _LIBFC_BASICOCTETARRAY_H_
#  define _LIBFC_BASICOCTETARRAY_H_

#  include <cstdint>
#  include <cstring>
#  include <string>

namespace LIBFC {

  /** An array of octets, as it may appear in an IPFIX message.
   *
   * This class is necessary to encapsulate the IPFIX types octetArray
   * and string, since they do not map to C++ basic data types.
   *
   * This class contains a buffer that will grow as needed, but that
   * will never shrink.
   */
  class BasicOctetArray {
  public:
    /** Creates an octet array. */
    BasicOctetArray();

    /** Destroys this octet array, releasing any storage. */
    ~BasicOctetArray();

    /** Returns the length of this octet array.
     *
     * @return the length of this octet array, in octets
     */
    size_t get_length() const;

    /** Returns the octets in this octet array.
     *
     * @return the octets in this octet array
     */
    const uint8_t* get_buf() const;

    /** Overwrites this octet array.
     *
     * This function replaces the previous contents of this octet
     * array with the octets given in the parameters. If necessary,
     * the buffer is resized to accomodate the new contents.
     *
     * @param buf the new contents
     * @param length the number of octets in the new content
     */
    void copy_content(const uint8_t* buf, size_t length);

    /** Converts the contents of this octet array to a string.
     *
     * This function makes sense only for debugging purposes and if
     * the contents of this octet array are in fact interpretable as a
     * printable string in the native character set.
     *
     * @return contents of this octet array, as a string
     */
    const std::string to_string() const;

  private:
    /** The buffer containing the content. */
    uint8_t* buf;

    /** The number of octets in the content. */
    size_t length;

    /** The capacity of the buffer. This must not be less than the
     * content's length. */
    size_t capacity;
  };

} // namespace LIBFC

#endif // _LIBFC_BASICOCTETARRAY_H_
