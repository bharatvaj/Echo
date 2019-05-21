#include <echo/EchoClient.hpp>
#include <clog/clog.h>
#include <echo/EchoWriter.hpp>
#include <echo/EchoProtocol.hpp>

static const char *TAG = "EchoClient";

echo::EchoClient *echo::EchoClient::instance = nullptr;

echo::EchoClient::EchoClient(){
    xs_init();
}

echo::EchoClient *echo::EchoClient::getInstance(){
  if (instance == nullptr) {
    instance = new EchoClient();
  }
  return instance;
}

void echo::EchoClient::deleteInstance(){
  if(instance != nullptr){
    delete instance;
  }
}

void echo::EchoClient::setServer(std::string s) { this->server = s; }

void echo::EchoClient::initialize() {
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

  const char *key = "hello";
  int keyLen = 5;

  /*
    Generate key here
    key = public key
    keyLen = strlen(public key)
  */

  ///create and send key chat
  Chat *c = createChat(userId.c_str(), "server", key, keyLen);
  xs_SOCKET ss = getServerSocket();
  if(!EchoWriter::getInstance()->write(ss, c)){
    clog_f(TAG, "Cannot connect to server");
    initCallback(nullptr);
    return;
  }

  clog_i(TAG, "Connected to server");

  Chat *c1 = EchoReader::getInstance()->read(getServerSocket());
  if(c1 == nullptr){
    clog_f(TAG, "Communication error");
    initCallback(nullptr);
    return;
  }

  clog_i(TAG, "Sent login ID + key");

  if(strncmp((char *)c1->chat, ECHO_AUTH_ERROR, strlen(ECHO_AUTH_ERROR)) == 0){
    clog_e(TAG, "Authentication error");
    initCallback(nullptr);
    return;
  } else if(strncmp((char *)c1->chat, ECHO_OK, strlen(ECHO_OK)) != 0){
    clog_e(TAG, "Protocol Mismatch");
    initCallback(nullptr);
    return;
  }

  clog_i(TAG, "Authentication Successful");

  readerThread = new std::thread([=](xs_SOCKET s){
    while(!stopRead){
      Chat *chat = EchoReader::getInstance()->read(s);
      if(chat == nullptr){
        //disconnected, call finish
        stopRead = true;
        finishCallback(this);
        return;
      }
      if(chat->isStream){
        fireEvent(EchoEvent::STREAM, chat);
      } else {
        fireEvent(EchoEvent::READ, chat);
      }
    }
  }, getServerSocket());

  initCallback(this);
}

// returns available server
xs_SOCKET echo::EchoClient::getServerSocket() {
  if (server.empty())
    return xs_ERROR;
  if (sock == xs_ERROR) {
    // connect
    sock = comm_connect_server(server.c_str(), ECHO_DEFAULT_PORT);
  }
  return sock;
}
