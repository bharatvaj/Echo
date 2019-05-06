#include <echo/EchoClient.hpp>
#include <clog/clog.h>

static const char *TAG = "EchoClient";

echo::EchoClient *echo::EchoClient::instance = nullptr;

echo::EchoClient *echo::EchoClient::getInstance(){

    if (instance == nullptr) {
      instance = new EchoClient();
    }
    return instance;
}

void echo::EchoClient::setServer(std::string server) { this->server = server; }

void echo::EchoClient::initialize(echo::Echo::InitCallback initCallback) {
  if (userId.empty()) {
    clog_f(TAG, "Username not set");
    status = -1;
    return;
  }
  if (server.empty()) {
    clog_f(TAG, "Server not set");
    status = -1;
    return;
  }
  // authenticate user with server
  /*connect to server*/
  xs_SOCKET sock = comm_connect_server(server.c_str(), ECHO_DEFAULT_PORT);
  if (sock == SOCKET_ERROR) {
    clog_f(TAG, "Server communication failed");
    return;
  }
  /*get login*/
  Chat *r = EchoReader::getInstance(userId)->read(sock);
  if (r == nullptr) {
    status = -1;
    clog_f(TAG, "Protocol Mismatch");
    return;
  }

  /*get private key*/
  // somehow get private key and store it in pKey
  const char *pKey = "";

  /*use pKey to encrypt r->chat and send to server*/
  // const char *en = (void *)encrypt(pkey, r->chat);
  // Chat *rep = createChat(userId, en, strlen(en));
  // bool written = EchoWriter::getInstance()->write(sock, rep);
  // if(!written){
  //   status = -1;
  //   clog_f(TAG, "Protocol Mismatch");
  //   return;
  // }

  /*read for reply*/
  Chat *r2 = EchoReader::getInstance(userId)->read(sock);
  if (r2 == nullptr) {
    status = -1;
    clog_f(TAG, "Protocol Mismatch");
    return;
  }
  if (strncmp((char *)r2->chat, "OK", 2) != 0) {
    clog_e(TAG, "Authentication failed");
    return;
  }
  if (initCallback != nullptr) {
    initCallback(*this);
  }
}

// returns available server
xs_SOCKET echo::EchoClient::getServerSocket() {
  if (server.empty())
    return SOCKET_ERROR;
  if (sock == SOCKET_ERROR) {
    // connect
    sock = comm_connect_server(server.c_str(), ECHO_DEFAULT_PORT);
  }
  return SOCKET_ERROR;
}