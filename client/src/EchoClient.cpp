#include <echo/EchoClient.hpp>
#include <clog/clog.h>
#include <echo/EchoWriter.hpp>
#include <echo/EchoProtocol.hpp>

static const char *TAG = "EchoClient";

echo::EchoClient *echo::EchoClient::instance = nullptr;

echo::EchoClient *echo::EchoClient::getInstance(){

    if (instance == nullptr) {
      instance = new EchoClient();
    }
    return instance;
}

void echo::EchoClient::setUser(std::string userId) { this->userId = userId; }

void echo::EchoClient::setServer(std::string server) { this->server = server; }

void echo::EchoClient::initialize(echo::Echo::InitCallback initCallback) {
  if (userId.empty()) {
    clog_f(TAG, "Username not set");
    status = -1;
    initCallback(nullptr);
    return;
  }
  if (server.empty()) {
    clog_f(TAG, "Server not set");
    status = -1;
    initCallback(nullptr);
    return;
  }
  // authenticate user with server
  /*connect to server*/
  xs_SOCKET sock = comm_connect_server(server.c_str(), ECHO_DEFAULT_PORT);
  if (sock == SOCKET_ERROR) {
    clog_f(TAG, "Server communication failed");
    initCallback(nullptr);
    return;
  }

  const char *key = "hello";
  int keyLen = 255;

  /*
    Generate key here
    key = public key
    keyLen = strlen(public key)
  */

  ///create and send key chat
  Chat *c = createChat(userId.c_str(), "server", key, keyLen);
  if(!EchoWriter::getInstance()->write(sock, c)){
    clog_f(TAG, "Cannot send key");
    initCallback(nullptr);
    return;
  }

  Chat *c1 = EchoReader::getInstance()->read(sock);
  if(c1 == nullptr){
    clog_f(TAG, "Communication error");
    initCallback(nullptr);
    return;
  }
  if(strcmp((char *)c1->chat, ECHO_AUTH_ERROR) == 0){
    clog_e(TAG, "Authentication error");
    initCallback(nullptr);
    return;
  } else if(strcmp((char *)c1->chat, ECHO_OK) != 0){
    clog_e(TAG, "Protocol Mismatch");
    initCallback(nullptr);
    return;
  }

  readerThread = new std::thread([](xs_SOCKET sock, ChatCallback readCallback){
    Chat *c = EchoReader::getInstance()->read(sock);
    readCallback(c);
  }, sock, readCallback);

  initCallback(this);
}

// returns available server
xs_SOCKET echo::EchoClient::getServerSocket() {
  if (server.empty())
    return SOCKET_ERROR;
  if (sock == SOCKET_ERROR) {
    // connect
    sock = comm_connect_server(server.c_str(), ECHO_DEFAULT_PORT);
  }
  return sock;
}
