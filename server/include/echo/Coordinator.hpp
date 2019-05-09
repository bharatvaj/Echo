#ifndef _ECHO_COORDINATOR_H
#define _ECHO_COORDINATOR_H
#include <echo/Echo.hpp>
#include <echo/TriggerService.hpp>
namespace echo {
  class Coordinator {
    Echo *echo = nullptr;
  public:
    Coordinator(Echo *);
    ~Coordinator();
  };
}
#endif
