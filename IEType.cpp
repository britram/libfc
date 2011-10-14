#include "IEType.h"

namespace IPFIX {

const IEType* IEType::octetArray() { 
  static OctetArrayType iet(std::string("octetArray"), kOctetArray, 0, kVarlen); 
  return &iet; 
}

const IEType* IEType::unsigned8() {
  static UnsignedType iet(std::string("unsigned8"), kUnsigned8, 1, 1); 
  return &iet;
}

const IEType* IEType::unsigned16() { 
  static UnsignedType iet(std::string("unsigned16"), kUnsigned16, 1, 2);
  return &iet;
}

const IEType* IEType::unsigned32() { 
  static UnsignedType iet(std::string("unsigned32"), kUnsigned32, 1, 4);
  return &iet;
}

const IEType* IEType::unsigned64() { 
  static UnsignedType iet(std::string("unsigned64"), kUnsigned64, 1, 8); 
  return &iet; 
}

const IEType* IEType::signed8() { 
  static SignedType iet(std::string("signed8"), kSigned8, 1, 1); 
  return &iet;
}

const IEType* IEType::signed16() { 
  static SignedType iet(std::string("signed16"), kSigned16, 1, 2); 
  return &iet; 
}

const IEType* IEType::signed32() { 
  static SignedType iet(std::string("signed32"), kSigned32, 1, 4); 
  return &iet; 
}

const IEType* IEType::signed64() { 
  static SignedType iet(std::string("signed64"), kSigned64, 1, 8); 
  return &iet; 
}

const IEType* IEType::float32() { 
  static Float32Type iet(std::string("float32"), kFloat32, 4, 4); 
  return &iet; 
}

const IEType* IEType::float64() { 
  static Float64Type iet(std::string("float64"), kFloat64, 4, 8); 
  return &iet; 
}

const IEType* IEType::boolean() { 
  static UnsignedType iet(std::string("boolean"), kBoolean, 1, 8); 
  return &iet; 
}

const IEType* IEType::macAddress() { 
  static OctetArrayType iet(std::string("macAddress"), kMacAddress, 6, 6); 
  return &iet; 
}

const IEType* IEType::string() { 
  static OctetArrayType iet(std::string("string"), kString, 0, kVarlen); 
  return &iet; 
}

const IEType* IEType::dateTimeSeconds() { 
  static UnsignedType iet(std::string("dateTimeSeconds"), kDateTimeSeconds, 4, 4); 
  return &iet; 
}

const IEType* IEType::dateTimeMilliseconds() { 
  static UnsignedType iet(std::string("dateTimeMilliseconds"), kDateTimeMilliseconds, 8, 8); 
  return &iet; 
}

const IEType* IEType::dateTimeMicroseconds() { 
  static MicrosecondsType iet(std::string("dateTimeMicroseconds"), kDateTimeMicroseconds, 8, 8); 
  return &iet;
}

const IEType* IEType::dateTimeNanoseconds() { 
  static MicrosecondsType iet(std::string("dateTimeNanoseconds"), kDateTimeNanoseconds, 8, 8); 
  return &iet; 
}

const IEType* IEType::ipv4Address() { 
  static UnsignedType iet(std::string("ipv4Address"), kIpv4Address, 4, 4); 
  return &iet; 
}

const IEType* IEType::ipv6Address() { 
  static OctetArrayType iet(std::string("ipv6Address"), kIpv6Address, 16, 16); 
  return &iet; 
}

const IEType* IEType::unknown() { 
  static OctetArrayType iet(std::string("unknown"), 0, 0, 0); 
  return &iet; 
}

}
