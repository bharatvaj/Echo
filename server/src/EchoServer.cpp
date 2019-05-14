#include <clog/clog.h>
#include <echo/EchoServer.hpp>
#include <echo/EchoWriter.hpp>
#include <echo/EchoReader.hpp>
#include <echo/UserDB.hpp>
#include <echo/DB.hpp>
#include <echo/EchoProtocol.hpp>
#include <echo/TriggerService.hpp>

static const char *TAG = "EchoServer";

void echo::EchoServer::sent(echo::Chat *chat){
  //chat delivered, safe to delete chat
  DB::getInstance()->deleteChat(chat);
}

void echo::EchoServer::sendError(echo::Chat *chat){
  // don't delete from db
  // just send again or close the connection
  // on the event of send fail this function is called,
  // possible recursive error
}

void echo::EchoServer::readCallback(echo::Chat *chat){
  clog_v(TAG, "[%s](%s => %s) %s", chat->id, chat->from, chat->to, chat->chat);
  DB::getInstance()->enqueueChat(chat);
  Echo *recv = TriggerService::getInstance()->isOnline(chat->to);
  if(recv == nullptr){
    //not online
    return;
  }
  recv->send(chat);
}

void echo::EchoServer::streamCallback(echo::Chat *chat){
  //forward
  send(chat);
}

echo::EchoServer::EchoServer(){
  getServerSocket();
}

echo::EchoServer *echo::EchoServer::getInstance() {
  EchoServer *echoServer = new EchoServer();
  return echoServer;
}

void echo::EchoServer::initialize() {

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

  echo::UserDB *db = echo::UserDB::getInstance();
  if(db->idExists(c->from)){
    clog_i(TAG, "User exists");
    if(!db->keyChecks(c->from, c->chat, c->chatLen)){
      Chat *ec = createChat(c->to, c->from, ECHO_AUTH_ERROR, sizeof(ECHO_AUTH_ERROR));
      clog_i(TAG, "Authentication error");
      EchoWriter::getInstance()->write(getServerSocket(), ec);
      initCallback(nullptr);
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

  userId = std::string(c->from);

  on(EchoEvent::STREAM, this, &EchoServer::streamCallback);
  on(EchoEvent::READ, this, &EchoServer::readCallback);
  on(EchoEvent::SENT, this, &EchoServer::sent);
  on(EchoEvent::SEND_ERR, this, &EchoServer::sendError);

  std::vector<Chat *> chats = DB::getInstance()->getChatsFor(userId.c_str());
  for(auto v : chats){
    clog_i(TAG, "Sending chat: %s", v->id);
    send(v);
  }

  readThread = new std::thread([=](xs_SOCKET sock){
    while(!stopRead){
      Chat *chat = EchoReader::getInstance()->read(sock);
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

xs_SOCKET echo::EchoServer::getServerSocket() {
  if (sock == SOCKET_ERROR) {
    sock = comm_start_server(ECHO_DEFAULT_PORT);
  }
  return sock;
}
