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

#ifndef _libfc_DECODEPLAN_H_
#  define _libfc_DECODEPLAN_H_

#  include <vector>

#  if defined(_libfc_HAVE_LOG4CPLUS_)
#    include <log4cplus/logger.h>
#  endif /* defined(_libfc_HAVE_LOG4CPLUS_) */

#  include "PlacementTemplate.h"
#  include "IETemplate.h"

namespace libfc {

  /** Decode plans describe how a data record is to be decoded.
   *
   * Decoding a data record means determining, for each data field, 
   *
   *   - if the data's endianness must be converted;
   *   - if the data needs to be transformed in any other way (for
   *     example, boolean values are encoded with 1 meaning true and 2
   *     meaning false(!!), or reduced-length encoding of floating-point
   *     values means that doubles are really transferred as floats);
   *   - for variable-length data, what the length of the encoded value
   *     is; and
   *   - where the converted data is to be stored.
   *
   * Basically, clients register sets of pairs of <ie, pointer> with the
   * IPFIXContentHandler class below.  This is called a Placement Template.
   * This placement template will then be used to match incoming data
   * records.  The previously used procedure was to nominate the first
   * placement template whose set of information elements is a subset of
   * the information elements for the data set in question. We
   * implement this here as well, but it might be changed easily.  (For
   * example, we might reasonably select that placement template that is a
   * subset of the data set's template and at the same time matches the
   * most fields.)
   *
   * Now having a template for the data set (called a Wire Template) and
   * a matching placement template, we create a Decoding Plan.  Basically,
   * a decoding plan is a sequence of decisions, one for each field.
   * There are two types of decisions:
   *
   *   - A SKIP decision causes the corresponding field to be skipped.
   *   - A TRANSFER decision causes the corresponding field to be
   *     endianness-converted if necessary (this can be gleaned form the
   *     corresponding information element's type), and copied to the
   *     pointer that the client gave at the time of registration.
   *
   * For convenience, there exist two variations of each decision,
   * depending on whether the corresponding field is fixed-length field
   * or a variable-length field.
   *
   * It would be nice if we could collapse two adjacent SKIP decisions
   * into one, but that can only be done if the two fields in question
   * are fixed-length fields.  We might do this in a future release if
   * it turns out to be a performance problem.
   */
  class DecodePlan {
  public:
    /** Creates a decoding plan from a placement template and a wire
     * template.
     *
     * @param placement_template a placement template that must have been
     *   found to match the wire template (all IEs in the placement
     *   template must also appear in the wire template)
     * @param wire_template the wire template for the data set
     */
    DecodePlan(const PlacementTemplate* placement_template,
               const IETemplate* wire_template);
    
    /** Executes the plan.
     *
     * Due to the construction of IPFIX (there exists variable-length
     * data and a data record does not have a header containing its
     * length), we do not know the exact length of that data record.
     * Hence we give the length of the remaining data set and expect
     * this member function to return the number of bytes that it has
     * decoded.
     *
     * @param buf the buffer containing the data record (and the
     *     remaining data set)
     * @param length length of the remaining data set
     *
     * @return number of bytes decoded
     */
    uint16_t execute(const uint8_t* buf, uint16_t length);
    
  private:
    struct Decision {
      /** The decision type. */
      enum decision_type_t {
        /** Skip a fixed amount. */
        skip_fixlen,
        
        /** Skip a variable amount. */
        skip_varlen,
        
        /** Transfer a fixed amount, with no endianness conversion, no
         * booleans, and no octet string. */
        transfer_fixlen,
        
        /** Transfer a boolean. Someone found it amusing in RFC 2579 to
         * encode the boolean values true and false as 1 and 2,
         * respectively [sic!].  And someone else found it amusing to
         * standardise this behaviour in RFC 5101 too.  This is of
         * course wrong, since it disallows entirely sensible operations
         * like `plus' for "or", `times' for "and" and `less than' for
         * implication (which is what you get when you make false less
         * than true). */
        transfer_boolean,
        
        /** Transfer a fixed amount, with endianness conversion. */
        transfer_fixlen_endianness,
        
        /** Transfer an octet string with fixed length. */
        transfer_fixlen_octets,
        
        /** Reduced-length float64, no endianness conversion. */
        transfer_float_into_double,
        
        /** Reduced-length float64, with endianness conversion. */
        transfer_float_into_double_endianness,
        
        /** Transfer a variable amount. */
        transfer_varlen,
      } type;
      
      /** How much data is affected in the data set?  This field makes
       * sense only in fixlen decisions. */
      uint16_t length;
      
      /** Destination type size in bytes.  This field makes sense only in
       * transfer_fixlen decisions. */
      uint16_t destination_size;
      
      /** Transfer target. This field makes sense only in transfer
       * decisions.  The caller must make sure that these pointers are
       * suitably aligned for the result data type (for fixlen
       * transfers), or that they point to a BasicOctetArray object (for
       * varlen transfers). */
      void* p;
      
      /** Original wire template IE. This field makes sense only in
       * transfer decisions. */
      const InfoElement* wire_ie;
      
      std::string to_string() const;
    };
    
    std::vector<Decision> plan;
    
#if defined(_libfc_HAVE_LOG4CPLUS_)
    log4cplus::Logger logger;
#endif /* defined(_libfc_HAVE_LOG4CPLUS_) */
  };
  
#endif /* _libfc_DECODEPLAN_H_ */

} /* namespace libfc */
