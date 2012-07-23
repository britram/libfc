/* Copyright (c) 2011-2012 ETH Zürich. All rights reserved.
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
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT 
 * HOLDERBE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
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
 *
 * @section DESCRIPTION
 *
 * This file specifies the interface to IEType, which models an IPFIX data 
 * type as defined in RFC 5101, 5102, and 5610. This interface is used primarily
 * by the IPFIX encoder and decoder internals.
 */

#ifndef IPFIX_IETYPE_H // idem
#define IPFIX_IETYPE_H // hack

#include <cstring>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

#include <boost/any.hpp>

#include "Constants.h"

namespace IPFIX {


class IEType {

public:

  /**
   * Get the number of types known to libfc. This won't change without updating RFC5101.
   */
  static size_t ieTypeCount();

  /** 
   * Get an IEType representing IPFIX octetArrays (see RFC 5101, 5102, 5610)
   * @return a pointer to the octetArray type
   */
  static const IEType* octetArray();

  /** 
   * Get an IEType representing IPFIX unsigned8 (see RFC 5101, 5102, 5610)
   * @return a pointer to the unsigned8 type
   */
  static const IEType* unsigned8();

  /** 
   * Get an IEType representing IPFIX unsigned16 (see RFC 5101, 5102, 5610)
   * @return a pointer to the unsigned16 type
   */
  static const IEType* unsigned16(); 

  /** 
   * Get an IEType representing IPFIX unsigned32 (see RFC 5101, 5102, 5610)
   * @return a pointer to the unsigned32 type
   */
  static const IEType* unsigned32(); 

  /** 
   * Get an IEType representing IPFIX unsigned64 (see RFC 5101, 5102, 5610)
   * @return a pointer to the unsigned64 type
   */
  static const IEType* unsigned64(); 

  /** 
   * Get an IEType representing IPFIX signed8 (see RFC 5101, 5102, 5610)
   * @return a pointer to the signed8 type
   */
  static const IEType* signed8(); 

  /** 
   * Get an IEType representing IPFIX signed16 (see RFC 5101, 5102, 5610)
   * @return a pointer to the signed16 type
   */
  static const IEType* signed16(); 

  /** 
   * Get an IEType representing IPFIX signed32 (see RFC 5101, 5102, 5610)
   * @return a pointer to the signed32 type
   */
  static const IEType* signed32(); 

  /** 
   * Get an IEType representing IPFIX signed64 (see RFC 5101, 5102, 5610)
   * @return a pointer to the signed64 type
   */
  static const IEType* signed64(); 

  /** 
   * Get an IEType representing IPFIX float32 (see RFC 5101, 5102, 5610)
   * @return a pointer to the float32 type
   */
  static const IEType* float32(); 

  /** 
   * Get an IEType representing IPFIX float64 (see RFC 5101, 5102, 5610)
   * @return a pointer to the float64 type
   */
  static const IEType* float64(); 

  /** 
   * Get an IEType representing IPFIX boolean (see RFC 5101, 5102, 5610)
   * @return a pointer to the boolean type
   */
  static const IEType* boolean(); 

  /** 
   * Get an IEType representing IPFIX macAddress (see RFC 5101, 5102, 5610)
   * @return a pointer to the macAddress type
   */
  static const IEType* macAddress(); 

  /** 
   * Get an IEType representing IPFIX string (see RFC 5101, 5102, 5610)
   * @return a pointer to the string type
   */
  static const IEType* string(); 
 
   /** 
   * Get an IEType representing IPFIX dateTimeSeconds (see RFC 5101, 5102, 5610)
   * @return a pointer to the dateTimeSeconds type
   */
  static const IEType* dateTimeSeconds(); 

  /** 
   * Get an IEType representing IPFIX dateTimeMilliseconds (see RFC 5101, 5102, 5610)
   * @return a pointer to the dateTimeMilliseconds type
   */
  static const IEType* dateTimeMilliseconds(); 

  /** 
   * Get an IEType representing IPFIX dateTimeMicroseconds (see RFC 5101, 5102, 5610)
   * @return a pointer to the dateTimeMicroseconds type
   */
  static const IEType* dateTimeMicroseconds();

  /** 
   * Get an IEType representing IPFIX dateTimeNanoseconds (see RFC 5101, 5102, 5610)
   * @return a pointer to the dateTimeNanoseconds type
   */
  static const IEType* dateTimeNanoseconds();

  /** 
   * Get an IEType representing IPFIX ipv4Address (see RFC 5101, 5102, 5610)
   * @return a pointer to the ipv4Address type
   */
  static const IEType* ipv4Address();

  /** 
   * Get an IEType representing IPFIX ipv6Address (see RFC 5101, 5102, 5610)
   * @return a pointer to the ipv6Address type
   */
  static const IEType* ipv6Address();

  /** 
   * Get an IEType representing an unknown IPFIX type. This is meant only for the internal use of libfc.
   * @return a pointer to the octetArray type
   */
  static const IEType* unknown();
  
  /**
   * Get the name of this type, as in RFC 5610.
   * @return the type name as a string
   */
  const std::string& name() const { return name_; }
  
  /**
   * Get the number of this type, as in RFC 5610.
   * @return the type number
   */
  unsigned int number() const { return number_; }
  
  /**
   * Determine if the type requires endian-swapping on en/decoding. 
   * Signed and unsigned integral types, and types derived therefrom, 
   * are generally endian, while types that specify
   * their own encoding are generally not.
   *
   * @return true if the type requires endian-swapping
   */
  bool isEndian() const { return endian_; }
  
  /**
   * Determine whether the given length is permissable for a given type. 
   * Used for sanity checking by the encoder and decoder. 
   *
   * @param len the length to check
   * @return true if the length is permissable
   */
  bool permitsLength(size_t len) const {
    return (len >= minlen_ && len <= maxlen_);
  }

protected:

  /** Convenience const for setting the endian flag in IEType constructors */
  static const bool kEndian = true;
  
  /** Convenience const for setting the endian flag in IEType constructors; the opposite of kEndian */
  static const bool kRaw = false;

  IEType(const std::string name, 
         unsigned int number,
         size_t minlen,
         size_t maxlen,
         bool endian):
    name_(name),
    number_(number),
    minlen_(minlen),
    maxlen_(maxlen),
    endian_(endian)
    {};

  virtual ~IEType() {}
  
private:
  std::string     name_;
  unsigned int    number_;
  size_t          minlen_;
  size_t          maxlen_;
  bool            endian_;
};

/**
 * IEType derived class for simple arrays of bytes
 */
class OctetArrayType: public IEType { 
public:
  OctetArrayType(const std::string name, 
                 unsigned int number,
                 size_t minlen,
                 size_t maxlen) :
    IEType(name, number, minlen, maxlen, IEType::kRaw) {};
  };

/**
 * IEType derived class for unsigned integers
 */
class UnsignedType: public IEType { 
public:
  UnsignedType(const std::string name, 
                 unsigned int number,
                 size_t minlen,
                 size_t maxlen) :
    IEType(name, number, minlen, maxlen, IEType::kEndian) {};
};

/**
 * IEType derived class for signed integers
 */
class SignedType: public IEType { 
public:
  SignedType(const std::string name, 
                 unsigned int number,
                 size_t minlen,
                 size_t maxlen) :
    IEType(name, number, minlen, maxlen, IEType::kEndian) {};
};

/**
 * IEType derived class for single-precision floating point
 */
class Float32Type: public IEType { 
public:
  Float32Type(const std::string name, 
                 unsigned int number,
                 size_t minlen,
                 size_t maxlen) :
    IEType(name, number, minlen, maxlen, IEType::kEndian) {};
};

/**
 * IEType derived class for double-precision floating point
 */
class Float64Type: public IEType { 
public:
  Float64Type(const std::string name, 
                 unsigned int number,
                 size_t minlen,
                 size_t maxlen) :
    IEType(name, number, minlen, maxlen, IEType::kEndian) {};
};

/**
 * IEType derived class for types using NTP encoding for time
 */
class MicrosecondsType: public IEType { 
public:
  MicrosecondsType(const std::string name, 
                 unsigned int number,
                 size_t minlen,
                 size_t maxlen) :
    IEType(name, number, minlen, maxlen, IEType::kEndian) {};
};

}

#endif // idem hack
