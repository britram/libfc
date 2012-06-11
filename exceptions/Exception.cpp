#include "Exception.h"

namespace IPFIX {

  Exception::Exception(const std::string& _message) : message(_message) {}

  Exception::~Exception() throw() {}

  const char* Exception::what() const throw() {
    return message.c_str();
  }

} // namespace IPFIX
