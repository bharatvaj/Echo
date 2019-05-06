#ifndef _ECHO_COMMON_H
#define _ECHO_COMMON_H
#include <comm.h>
#include <iostream>

namespace echo {
// change to singleton
class Common {
public:
  static std::string userId;
};
} // namespace echo
#endif