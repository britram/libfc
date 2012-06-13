/* Hi Emacs, please use -*- mode: C++; -*- */
#ifndef IPFIX_EXCEPTION_H
#  define IPFIX_EXCEPTION_H

#  include <stdexcept>
#  include <string>

namespace IPFIX {

  /** Exception base class.
   */
  class Exception : public std::exception {
  public:
    explicit Exception(const std::string& message);
    ~Exception() throw();
    const char* what() const throw();
  private:
    std::string message;
  };

} // namespace IPFIX

#endif /* IPFIX_EXCEPTION_H */
