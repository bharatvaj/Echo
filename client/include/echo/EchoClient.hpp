#ifndef _ECHO_CLIENT_H
#define _ECHO_CLIENT_H
#include <echo/Echo.hpp>
namespace echo {
class EchoClient : public Echo {
  static EchoClient *instance;
  std::thread *readerThread;
  std::string userId;
  std::string server;

public:
  static EchoClient *getInstance();

  void setUser(std::string userId);
  void setServer(std::string);

  void initialize(InitCallback) override;
  xs_SOCKET getServerSocket() override;
};
} // namespace echo

#endif
