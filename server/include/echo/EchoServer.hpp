#ifndef _ECHO_SERVER_H
#define _ECHO_SERVER_H
#include <comm.h>
#include <echo/Echo.hpp>
#include <vector>
namespace echo {
class EchoServer : public Echo {
  EchoServer();
  std::thread *readThread = nullptr;
  bool stopRead = false;

  void sent(echo::Chat *chat);
  void sendError(echo::Chat *chat);
  void readCallback(echo::Chat *chat);
  void streamCallback(echo::Chat *chat);

public:
  static EchoServer *getInstance();

  void initialize() override;
  xs_SOCKET getServerSocket() override;
};
} // namespace echo

#endif
