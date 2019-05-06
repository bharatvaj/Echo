#include <clog/clog.h>
#include <echo/Echo.hpp>
#include <echo/EchoWriter.hpp>

static const char *TAG = "Echo";

echo::Echo *echo::Echo::instance = nullptr;
xs_SOCKET echo::Echo::sock = SOCKET_ERROR;

#define ECHO_DEFAULT_PORT 4500

// returns available server
xs_SOCKET echo::Echo::getServerSocket() {
  if (server.empty())
    return SOCKET_ERROR;
  if (sock == SOCKET_ERROR) {
    // connect
    sock = comm_connect_server(server.c_str(), ECHO_DEFAULT_PORT);
  }
  return SOCKET_ERROR;
}

echo::Echo *echo::Echo::getInstance() {
  if (instance == nullptr)
    instance = new Echo();
  return instance;
}

void echo::Echo::initialize(echo::Echo::InitCallback initCallback) {
  if (echo::Common::userId.empty()) {
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
  Chat *r = EchoReader::getInstance()->read(sock);
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
  // Chat *rep = createChat(en, strlen(en));
  // bool written = EchoWriter::getInstance()->write(sock, rep);
  // if(!written){
  //   status = -1;
  //   clog_f(TAG, "Protocol Mismatch");
  //   return;
  // }

  /*read for reply*/
  Chat *r2 = EchoReader::getInstance()->read(sock);
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

void echo::Echo::setReadCallback(ChatCallback callback) {
  this->readCallback = callback;
}
void echo::Echo::setStreamCallback(ChatCallback callback) {
  this->streamCallback = callback;
}
void echo::Echo::setSendErrorCallback(ChatCallback callback) {
  this->sendErrorCallback = callback;
}
void echo::Echo::setSentCallback(ChatCallback callback) {
  this->sentCallback = callback;
}

void echo::Echo::setServer(std::string server) { this->server = server; }

void echo::Echo::setUser(std::string userId) { echo::Common::userId = userId; }

void echo::Echo::send(std::string id, Chat *chat) {
  if (chat == nullptr) {
    // if (sendErrorCallback != nullptr)
    sendErrorCallback(chat);
    return;
  }
  EchoWriter::getInstance()->write(getServerSocket(), chat);
}

void echo::Echo::block(std::string str) {
  // TODO
}

void echo::Echo::unblock(std::string userId) {
  // TODO
}

std::vector<std::string> echo::Echo::getBlockedUsers() {
  return std::vector<std::string>();
}

int echo::Echo::waitForClose() {
  std::unique_lock<std::mutex> lck(closeLck);
  closeCv.wait(lck, [=] { return shouldClose; });
  return status;
}

void echo::Echo::close() {
  if (sock != SOCKET_ERROR) {
    comm_close_socket(sock);
  }
  shouldClose = true;
  closeCv.notify_all();
}