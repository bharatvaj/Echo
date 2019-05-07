#include <clog/clog.h>
#include <echo/EchoServer.hpp>
#include <echo/EchoWriter.hpp>
#include <echo/EchoReader.hpp>
#include <echo/DB.hpp>
#include <echo/EchoProtocol.hpp>

static const char *TAG = "EchoServer";

echo::EchoServer::EchoServer(){
  getServerSocket();
}

echo::EchoServer *echo::EchoServer::getInstance() {
  EchoServer *echoServer = new EchoServer();
  return echoServer;
}

void echo::EchoServer::initialize(echo::Echo::InitCallback initCallback) {

  if(sock == SOCKET_ERROR){
    clog_f(TAG, "Server instance failed");
    initCallback(nullptr);
    return;
  }

  Chat *c = EchoReader::getInstance()->read(sock);
  if(c == nullptr){
    clog_f(TAG, "Protocol mismatch");
    initCallback(nullptr);
    return;
  }
  clog_i(TAG, "Starting server instance");

  echo::DB *db = echo::DB::getInstance();
  if(db->idExists(c->from)){
    clog_i(TAG, "User exists");
    if(!db->keyChecks(c->from, c->chat, c->chatLen)){
      Chat *ec = createChat(c->to, c->from, ECHO_AUTH_ERROR, sizeof(ECHO_AUTH_ERROR));
      clog_i(TAG, "Authentication error");
      if(!EchoWriter::getInstance()->write(getServerSocket(), ec)){
        clog_f(TAG, "Write error");
        initCallback(nullptr);
      }
    }
  } else {
    db->saveKey(c->from, c->chat, c->chatLen);
    clog_i(TAG, "Saving key for new user");
  }
  Chat *okc = createChat(c->to, c->from, ECHO_OK, sizeof(ECHO_OK));
  if(!EchoWriter::getInstance()->write(getServerSocket(), okc)){
    clog_f(TAG, "Write error");
    initCallback(nullptr);
    return;
  }
  clog_i(TAG, "User authenticated");

  initCallback(this);

}

xs_SOCKET echo::EchoServer::getServerSocket() {
  if (sock == SOCKET_ERROR) {
    sock = comm_start_server(ECHO_DEFAULT_PORT);
  }
  return sock;
}
