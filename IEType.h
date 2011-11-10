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

// FIXME add an IEValue (or IEType::Value) class for representing values in a std::map 
// for the flexible, non-transcoding version of the interface

#ifndef IPFIX_IETYPE_H // idem
#define IPFIX_IETYPE_H // hack

#include <string>
#include <cstring>
#include <vector>
#include <map>
#include <stdexcept>
#include <boost/any.hpp>

#include "Constants.h"

namespace IPFIX {

/**
 * A logic error indicating a programming fault concerning the handling of IPFIX types
 */
class IETypeError : public std::logic_error {
public:
  explicit IETypeError(const std::string& what_arg): 
    std::logic_error(what_arg) {}
};

class IEType {

public:

  /** The number of types known to libfc. This won't change without updating RFC5101 */
  static const size_t kIETypeCount = 20;

  static const unsigned int kOctetArray = 0;
  static const unsigned int kUnsigned8 = 1;
  static const unsigned int kUnsigned16 = 2;
  static const unsigned int kUnsigned32 = 3;
  static const unsigned int kUnsigned64 = 4;
  static const unsigned int kSigned8 = 5;
  static const unsigned int kSigned16 = 6;
  static const unsigned int kSigned32 = 7;
  static const unsigned int kSigned64 = 8;
  static const unsigned int kFloat32 = 9;
  static const unsigned int kFloat64 = 10;
  static const unsigned int kBoolean = 11;
  static const unsigned int kMacAddress = 12;
  static const unsigned int kString = 13;
  static const unsigned int kDateTimeSeconds = 14;
  static const unsigned int kDateTimeMilliseconds = 15;
  static const unsigned int kDateTimeMicroseconds = 16;
  static const unsigned int kDateTimeNanoseconds = 17;
  static const unsigned int kIpv4Address = 18;
  static const unsigned int kIpv6Address = 19;

  /** Convenience const for setting the endian flag in IEType constructors */
  static const bool kEndian = true;
  
  /** Convenience const for setting the endian flag in IEType constructors; the opposite of kEndian */
  static const bool kRaw = false;

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
    IEType(name, number, minlen, maxlen, IEType::kRaw) {};
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
    IEType(name, number, minlen, maxlen, IEType::kRaw) {};
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
