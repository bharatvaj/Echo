#ifndef _ECHO_SERVER_H
#define _ECHO_SERVER_H
#include <comm.h>
#include <echo/Echo.hpp>
#include <vector>
namespace echo {
class EchoServer : public Echo {
  EchoServer();
public:
  static EchoServer *getInstance();

  void initialize() override;
  xs_SOCKET getServerSocket() override;
};
} // namespace echo

#endif
