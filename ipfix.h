/* Hi Emacs, please use -*- mode: C; -*- */
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
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL ETH
 * ZÜRICH BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 */
/**
 * @file ipfix.h
 * @brief C binding for IPFIX collection and export.
 * @author Stephan Neuhaus <neuhaust@tik.ee.ethz.ch>
 *
 * This file contains the struct definitions and function declarations
 * that are necessary to use libfc from C.
 *
 * This library supports reading (collection) and writing (export) of
 * IPFIX messages.  IPFIX is a data format (some say it is a protocol)
 * for exchanging computer network measurement data; it is defined in
 * RFC 5101 and others.
 *
 * The following text is taken from the documentation for the
 * PlacementTemplate class; it is replicated here for better
 * accessibility.  You do not need to know what a PlacementTemplate is
 * in order to understand the following text.
 *
 * IPFIX is sometimes called a <em>self-describing format</em>.  In
 * order to self-describe, an IPFIX message contains <em>template
 * records</em> that describe the format of the <em>data
 * records</em>, which contain the content.  A template record
 * basically says, "Hi, I'm a template record with the identifying
 * number 1234.  Later in this message, there may be collection of
 * data records, called a <em>data set</em>, also having the
 * identifying number 1234.  This means that the records in that
 * data set will have the following structure: [...]" That structure
 * is then described by giving a sequence of <em>information
 * elements</em> (and their encoded lengths as they appear on the
 * wire).  For example, a simplified version of a flow template
 * record could look like this:
 *
 * <table>
 *   <tr><th>IE name</th><th>length</th></tr>
 *   <tr><td>flowStartMilliseconds</td><td>8</td></tr>
 *   <tr><td>flowEndMilliseconds</td><td>8</td></tr>
 *   <tr><td>sourceIPv4Address</td><td>4</td></tr>
 *   <tr><td>destinationIPv4Address</td><td>4</td></tr>
 *   <tr><td>sourceTransportPort</td><td>2</td></tr>
 *   <tr><td>destinationTransportPort</td><td>2</td></tr>
 *   <tr><td>protocolIdentifier</td><td>1</td></tr>
 *   <tr><td>octetDeltaCount</td><td>4</td></tr>
 * </table>
 *
 * (This is a simplified template record because a real record would
 * contain not the information element names, but rather some
 * identifying numbers.)
 *
 * Then this means that later data sets with this template ID will
 * contain the given information elements in this order, with this
 * length.  Now it would be nice if we could read in a data record
 * and map it to a struct, simply by casting a pointer to the data
 * read from the data source.  But that is not possible for a number
 * of reasons:
 *
 *  - The data might need to undergo <em>endianness conversion</em>.
 *    For example, numbers will appear on the wire in network byte
 *    order (big endian), but the native format will be host byte
 *    order, which might be little endian.  This conversion affects
 *    some, but not all, information elements.  For example, the
 *    flowStartMilliseconds and sourceIPv4Address information
 *    elements are affected, whereas a sourceIPv6Address information
 *    element would not be affected.
 *
 *  - Some information elements have variable length; this is
 *    known as <em>varlen encoding</em>.  Obviously, varlen-encoded
 *    information elements cannot be directly mapped to structs,
 *    because structs have a fixed length, available with the
 *    <code>sizeof</code> operator.
 *
 *  - Some information element types cannot be directly mapped to
 *    native C++ data types.  Examples would be the
 *    <tt>octetArray</tt> and <tt>string</tt> data types.
 *
 *  - Information elements can be encoded on the wire with a length
 *    that is less than the length that would be implied by the
 *    information element's type.  This is called <em>reduced-length
 *    encoding</em>.  For example, the octetDeltaCount is of type
 *    <tt>unsigned64</tt>, but if only 32 of those 64 bits would be
 *    needed in a particular application, then the information
 *    element may appear on the wire with a length of just four
 *    octets.
 *
 *  - In a data record, the information elements follow one another
 *    without any <em>padding</em>. This can lead to data that is
 *    not suitably aligned.  For example, the protocolIdentifier
 *    information element above is only 1 octet long, causing the
 *    following octetDeltaCount information element to be aligned on
 *    an odd address (assuming that the base address was aligned on
 *    an even address).
 *
 *  - A collection process might not be interested in all
 *    information elements, but only in a <em>selection</em>, so
 *    forcing it to receive the entire data record would be
 *    unreasonable.
 *
 *  - The collection process might, for its very own reasons, want
 *    to <em>scatter</em> the information elements in its address
 *    space, not keeping them together as in a struct.
 *
 * An IPFIX collection process will therefore ultimately need to
 * decide where an information element as it appears in an IPFIX
 * data record will have to be stored in memory, just as an IPFIX
 * exporter will have to decide where an information element that is
 * to be exported will take its data from. This class provides the
 * association between a set of information elements and
 * corresponding memory locations.
 *
 * Here the excerpt from the PlacementTemplate documentation ends.
 *
 * Let's say that you have a file called "test.ipfix", containing
 * IPFIX messages, and that you want to extract IP source and
 * destination addresses from it. Here is how you could do that (with
 * apologies to only rudimentary error handling):
 *
 * @code
 * static uint32_t sip;
 * static uint32_t dip;
 *
 * static void callback(const struct ipfix_template_t* p) {
 *   printf("Got new values %08x and %08x\n", sip, dip);
 * }
 *
 * int main() {
 *   int fd = open("test.ipfix", O_RDONLY);
 *   if (fd < 0)
 *     exit(1);
 *
 *   struct ipfix_template_set_t* s = ipfix_template_set_new();
 *   struct ipfix_template_t* t = ipfix_template_new(s);
 *   ipfix_register_placement(t, "sourceIPv4Address", &sip, 0);
 *   ipfix_register_placement(t, "destinationIPv4Address", &dip, 0);
 *   ipfix_register_callback(s, callback);
 *   ipfix_collect_from_file(fd, s);
 *
 *   if (close(fd) < 0)
 *     exit(1);
 *
 *   ipfix_template_set_delete(s);
 *
 *   return 0;
 * }
 * @endcode
 */

#ifndef _LIBFC_IPFIX_H_
#  define _LIBFC_IPFIX_H_

#  if defined(__cplusplus)
extern "C" {
#  endif /* defined(__cplusplus) */

#  include <stdlib.h>

  /** An IPFIX template set.
   *
   * This is an opaque struct, and you should only ever handle a
   * pointer to it.
   */
  struct ipfix_template_set_t;

  /** An IPFIX template.
   *
   * This is an opaque struct, and you should only ever handle a
   * pointer to it.
   */
  struct ipfix_template_t;

  /** Creates a new IPFIX template set.
   *
   * @return a new IPFIX template set.
   */
  extern struct ipfix_template_set_t* ipfix_template_set_new();

  /** Creates a new IPFIX template within a template set.
   *
   * @param s the template set in which to create the new template
   * @return a new IPFIX template.
   */
  extern struct ipfix_template_t* ipfix_template_new(
    struct ipfix_template_set_t* s);

  /** Destroys an IPFIX template set.
   *
   * This also destroys all templates in the template set.
   *
   * @param s IPFIX template set, previously created by
   * ipfix_template_set_new().
   */
  extern void ipfix_template_set_delete(struct ipfix_template_set_t* s);

  /** Registers IE name/address association for .
   *
   * @param t the template in which to register the association
   * @param ie_name name of IPFIX information element.  For the
   *   moment, this name can only come from the standard information
   *   model, until I figure out a good way to support more
   *   information models.
   * @param p the address with which the previously mentioned
   *   information element is associated.
   * @param size the size of the information element on the wire, or
   *   0 for the default size (this can be used, for example, when
   *   collecting)
   *
   * @return non-zero if the operation was successful, 0 if the
   *     given size is not appropriate for the information element.
   */
  extern int ipfix_register_placement(struct ipfix_template_t* t,
                                      const char* ie_name, void* p, size_t size);

  /** Registers callback for when placement is complete.
   *
   * This function registers a callback that is called whenever a
   * record has been read that matches any template in the set.  In that
   * case, the memory addresses in the template will have been filled
   * with new values.
   *
   * @param s the template set
   * @param c the callback to call
   */
  extern void ipfix_register_callback(struct ipfix_template_t* s,
                                      void (*c) (const struct ipfix_template_t*));

  /** Collect IPFIX data from a file.
   *
   * @param fd a valid file descriptor, such as you'd get back from a
   *     successful call to open(2)
   * @param s template set containing the templates of interest and the callback
   *
   * @return non-zero on success and 0 on error
   */
  extern int ipfix_collect_from_file(int fd, struct ipfix_template_set_t* t);

   
#  if defined(__cplusplus)
}
#  endif /* defined(__cplusplus) */

#endif /* _LIBFC_IPFIX_H_ */
