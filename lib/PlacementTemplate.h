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

#ifndef _LIBFC_PLACEMENTTEMPLATE_H_
#  define _LIBFC_PLACEMENTTEMPLATE_H_

#  include <list>
#  include <map>
#  include <set>

#  ifdef _LIBFC_HAVE_LOG4CPLUS_
#    include <log4cplus/logger.h>
#  endif /* _LIBFC_HAVE_LOG4CPLUS_ */

#  include "InfoElement.h"
#  include "IETemplate.h"

namespace LIBFC {

  /** Association between IEs and memory locations.
   *
   * @section INTRODUCTION
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
   * Here is how it works.  Let's say you're interested in flow
   * records like the one shown in the above table, but you're only
   * interested in the source and destination IP addresses, and not
   * the other information elements.  We encapsulate this in a data
   * structure that we call a <em>placement template</em>, and this
   * code would let you express that interest:
   *
   * @code
   * uint32_t sip;
   * uint32_t dip;
   * LIBFC::InfoModel& model = IPFIX::InfoModel::instance();
   *
   * PlacementTemplate* my_flow_template = new PlacementTemplate();
   * 
   * my_flow_template->register_placement(
   *    model.lookupIE("sourceIPv4Address"),
   *    &sip);
   * my_flow_template->register_placement(
   *    model.lookupIE("destinationIPv4Address"),
   *    &dip);
   * @endcode
   *
   *
   * @section COLLECTION
   *
   * Now before you can start collecting records with these
   * information elements, you need two more things: first, you need
   * to tell someone that you want this placement template to go into
   * effect, and second, you need a way for that someone to tell you
   * that it's just read a record that matched this placement template
   * and that it has placed the values form the data record into the
   * pointers that you have provided in that placement template.  All
   * this is provided by a custom class derived from
   * PlacementCollector (we will be using the IPFIX namespace for
   * simplicity):
   *
   * @code
   * class MyCollector : public PlacementCollector {
   * public:
   *   MyCollector() {
   *      // Create and fill my_flow_template as above
   *
   *      register_placement_template(my_flow_template);
   *   }
   *
   *   void end_placement(const PlacementTemplate* tmpl) {
   *     // At this point, there are fresh values in this->sip
   *     // and this->dip
   *   }
   *
   * private:
   *  uint32_t sip;
   *  uint32_t dip;
   * };
   * 
   * MyCollector cb;
   *
   * int fd = open(filename.c_str(), O_RDONLY);
   * if (fd >= 0) {
   *   FileInputSource is(fd);
   *   try {
   *     cb.collect(is);
   *   } catch (FormatError e) {
   *     std::cerr << "Format error: " << e.what() << std::endl;
   *   }
   *   (void) close(fd);
   * }
   * @endcode
   *
   * And that's all, folks!
   *
   * It should be noted that it is possible to register more than one
   * placement template in the MyCollector constructor:
   *
   * @code
   *   MyCollector() {
   *     PlacementTemplate* my_flow_template = new PlacementTemplate();
   *     // Fill my_flow_template as above
   *      register_placement_template(my_flow_template);
   *
   *     PlacementTemplate* my_observation_template = new PlacementTemplate();
   *     // Fill my_obsrevation_template similarly
   *      register_placement_template(my_observation_template);
   *   }
   * @endcode
   *
   * This is the reason why the end_placement() member function has a
   * PlacementTemplate pointer parameter: so that you can distinguish
   * which of your templates has just been matched and hence which of
   * your data members now have fresh content.  Obviously, for this,
   * the template pointers should be data members of MyCollector.
   *
   * @section EXPORT
   *
   * Placement templates can also be used for export.  In fact, export
   * using the placement interface is slightly simpler than
   * collection, since no custom derived class is needed. (The reason
   * for this is that export needs no callback function, since the
   * information flow is now from the program to the IPFIX messages.)
   *
   * Let's say you want to write IPFIX records to a file, and that you
   * have a file descriptor for that file in the variable
   * <code>fd</code>.  Here is how you could export flow records:
   *
   * @code
   * FileExportDestination d(fd);
   * PlacementExporter exporter(d, 0x12344321);
   *
   * uint64_t flow_start_milliseconds = ...;
   * uint64_t flow_end_milliseconds = ...;
   * uint32_t source_ip_v4_address = ...;
   * uint32_t destination_ip_v4_address = ...;
   * uint16_t source_transport_port = ...;
   * uint16_t destination_transport_port = ...;
   * uint8_t  protocol_identifier = ...;
   * uint64_t octet_delta_count = ...;
   *
   * PlacementTemplate* my_flow_template = new PlacementTemplate();
   * // Fill template as above
   *
   * exporter.place_values(my_flow_template);
   *
   * // more calls to place_values()
   *
   * exporter.flush();
   *
   * delete my_flow_template;
   * @endcode
   *
   * And that's all!
   */
  class PlacementTemplate {
  public:
    /** Information associated with an InfoElement in a PlacementTemplate. */
    PlacementTemplate();

    ~PlacementTemplate();

    /** Registers an association between an IE and a memory location.
     *
     * @param ie the information element
     * @param p the memory location to be associated with the IE
     * @param size the size of the information element on the wire, or
     *     0 for the default size (this can be used, for example, when
     *     collecting) 
     *
     * @return true if the operation was successful, false if the
     *     given size is not appropriate for the information element.
     */
    bool register_placement(const InfoElement* ie, void* p, size_t size);

    /** Retrieves the memory location given an IE.
     *
     * @param ie the information element to look for
     * @param p pointer to the the memory location associated with
     *     that information element
     * @param size pointer to the size of the information element, or
     *     NULL if the size isn't requested
     *
     * @return true if the information element was found, false if
     *     the information element hasn't been registered previously.
     */
    bool lookup_placement(const InfoElement* ie, void** p, size_t* size) const;

    /** Tells whether a given template matches this template.
     *
     * A template T matches this template iff T's set of IEs is a
     * subset of this template's set of IEs.
     *
     * @param t the template to match
     * @param unmatched pointer to a set of InfoElements; if non-null,
     *   @emph{and} if the return value was greater than zero, this
     *   set will contain those IEs that were in T, but not in this
     *   template.  This will inform you about those IEs that weren't
     *   matched (and that you will therefore miss).
     *
     * @return if the templates match, the number of IEs in this
     *   template, 0 otherwise
     */
    unsigned int is_match(const IETemplate* t,
			  std::set<const InfoElement*>* unmatched) const;

    /** Creates a wire template suitable to represent this template
     * on the wire in a template record.
     *
     * @param template_id the template id to use for this set
     * @param buf pointer to a buffer where the template will be stored
     * @param size size of buffer
     */
    void wire_template(uint16_t template_id, 
                       const uint8_t** buf,
                       size_t* size) const;

    /** Computes the size of the current data record.
     *
     * This method can only meaningfully be called after all the
     * memory locations belonging to this template have valid values.
     * This method then computes the size of a data record given the
     * current values for the IEs.  This is only an issue for
     * varlen-encoded IEs, since all others can obviously be computed
     * without knowing the actual values.
     *
     * @return data record size, in octets
     */
    size_t data_record_size() const;

    /** Returns the template id.
     *
     * @return the template id, if it has been set, or 0 otherwise.
     */
    uint16_t get_template_id() const;

    /** Returns an iterator over the InfoElements in this template.
     *
     * @return an iterator pointing to the first information element.
     */
    std::list<const InfoElement*>::const_iterator begin() const;

    /** Returns an iterator to the end of the InfoElements in this template.
     *
     * @return an iterator pointing to the end of the information elements.
     */
    std::list<const InfoElement*>::const_iterator end() const;

  private:
    class PlacementInfo;
    std::map<const InfoElement*, PlacementInfo*> placements;

    /** List of information elements; needed for iteration. */
    std::list<const InfoElement*> ies;

    /** List of varlen IEs */
    std::list<const PlacementInfo*> varlen_ies;

    /** Representation of this template for a message. */
    mutable uint8_t* buf;

    /** Size of this template's representation for a message. */
    mutable size_t size;

    /** Sum of fixlen data item sizes in the data record representation. */
    mutable size_t fixlen_data_record_size;

    /** The template ID for the wire representation of this template. */
    mutable uint16_t template_id;

#  ifdef _LIBFC_HAVE_LOG4CPLUS_
    log4cplus::Logger logger;
#  endif /* _LIBFC_HAVE_LOG4CPLUS_ */
  };

} // namespace LIBFC
#endif // _LIBFC_PLACEMENTTEMPLATE_H_
