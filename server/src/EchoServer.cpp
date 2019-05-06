#include <clog/clog.h>
#include <echo/EchoServer.hpp>

static const char *TAG = "EchoServer";

std::vector<echo::EchoServer *> echo::EchoServer::instances =
    std::vector<EchoServer *>();

echo::EchoServer::EchoServer(xs_SOCKET sock) { this->sock = sock; }

echo::EchoServer *echo::EchoServer::getInstance(xs_SOCKET sock) {
  EchoServer *echoServer = new EchoServer(sock);
  instances.push_back(echoServer);
  return echoServer;
}

void echo::EchoServer::initialize(echo::Echo::InitCallback initCallback) {
  // if (userId.empty()) {
  //   clog_f(TAG, "Username not set");
  //   status = -1;
  //   return;
  // }
  //
}

// returns available server
xs_SOCKET echo::EchoServer::getServerSocket() {
  //
  return sock;
}