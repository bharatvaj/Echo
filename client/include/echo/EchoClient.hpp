#ifndef _ECHO_CLIENT_H
#define _ECHO_CLIENT_H
#include <echo/Echo.hpp>
namespace echo {
class EchoClient : public Echo {
  static EchoClient *instance;
  std::thread *readerThread;
  std::string server;
  bool stopRead = false;

public:
  static EchoClient *getInstance();
  static void deleteInstance();

  void setServer(std::string);

  void initialize() override;
  xs_SOCKET getServerSocket() override;
};
} // namespace echo

#endif
