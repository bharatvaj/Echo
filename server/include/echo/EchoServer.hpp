#ifndef _ECHO_SERVER_H
#define _ECHO_SERVER_H
#include <comm.h>
#include <echo/Echo.hpp>
#include <vector>
namespace echo {
class EchoServer : public Echo {
  static std::vector<EchoServer *> instances;

public:
  static EchoServer *getInstance(xs_SOCKET);
  EchoServer(xs_SOCKET sock);

  void initialize(InitCallback) override;
  xs_SOCKET getServerSocket() override;
};
} // namespace echo

#endif