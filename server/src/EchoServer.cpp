#include <clog/clog.h>
#include <echo/EchoServer.hpp>
#include <echo/EchoWriter.hpp>
#include <echo/EchoReader.hpp>
#include <echo/DB.hpp>
#include <echo/EchoProtocol.hpp>

static const char *TAG = "EchoServer";

echo::EchoServer::EchoServer(xs_SOCKET sock) { this->sock = sock; }

echo::EchoServer *echo::EchoServer::getInstance(xs_SOCKET sock) {
  EchoServer *echoServer = new EchoServer(sock);
  return echoServer;
}

void echo::EchoServer::initialize(echo::Echo::InitCallback initCallback) {
  // Chat *rep = createChat(userId, en, strlen(en));
  // bool written = EchoWriter::getInstance()->write(sock, rep);
  // if(!written){
  //   status = -1;
  //   clog_f(TAG, "Protocol Mismatch");
  //   return;
  // }
  xs_SOCKET sock = comm_start_server(ECHO_DEFAULT_PORT);
  if (sock == SOCKET_ERROR) {
    clog_f(TAG, "Cannot start server");
    initCallback(nullptr);
    return;
  }
  Chat *c = EchoReader::getInstance()->read(sock);
  if(c == nullptr){
    clog_f(TAG, "Protocol Mistmatch");
    initCallback(nullptr);
    return;
  }
  echo::DB *db = echo::DB::getInstance();
  if(db->idExists(c->from)){
    if(!db->keyChecks(c->from, c->chat, c->chatLen)){
      Chat *ec = createChat(c->to, c->from, ECHO_AUTH_ERROR, sizeof(ECHO_AUTH_ERROR));
      if(!EchoWriter::getInstance()->write(sock, ec)){
        clog_f(TAG, "Write error");
        initCallback(nullptr);
      }
    }
  } else {
    db->saveKey(c->from, c->chat, c->chatLen);
  }
  Chat *okc = createChat(c->to, c->from, ECHO_OK, sizeof(ECHO_OK));
  if(!EchoWriter::getInstance()->write(sock, okc)){
    clog_f(TAG, "Write error");
    initCallback(nullptr);
  }

  initCallback(this);

}

// returns available server
xs_SOCKET echo::EchoServer::getServerSocket() {
  //
  return sock;
}
