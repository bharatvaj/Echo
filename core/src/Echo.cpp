#include <clog/clog.h>
#include <echo/Echo.hpp>
#include <echo/EchoWriter.hpp>
#include <comm.h>
#include <crosssocket.h>


static const char *TAG = "Echo";

void echo::Echo::setInitCallback(EchoCallback callback){
  this->initCallback = callback;
}

void echo::Echo::setFinishCallback(EchoCallback callback){
  this->finishCallback = callback;
}

void echo::Echo::setReadCallback(ChatCallback callback)
{
  this->readCallback = callback;
}
void echo::Echo::setStreamCallback(ChatCallback callback)
{
  this->streamCallback = callback;
}
void echo::Echo::setSendErrorCallback(ChatCallback callback)
{
  this->sendErrorCallback = callback;
}
void echo::Echo::setSentCallback(ChatCallback callback)
{
  this->sentCallback = callback;
}

void echo::Echo::send(Chat *chat)
{
  if (chat == nullptr)
  {
    sendErrorCallback(chat);
    return;
  }
  if(!EchoWriter::getInstance()->write(getServerSocket(), chat)){
    sendErrorCallback(chat);
    return;
  }
  sentCallback(chat);
}

void echo::Echo::block(std::string str)
{
  // TODO
}

void echo::Echo::unblock(std::string userId)
{
  // TODO
}

std::vector<std::string> echo::Echo::getBlockedUsers()
{
  // TODO
  return std::vector<std::string>();
}

int echo::Echo::waitForClose()
{
  std::unique_lock<std::mutex> lck(closeLck);
  closeCv.wait(lck);
  return status;
}

void echo::Echo::close()
{
  if (sock != SOCKET_ERROR)
  {
    comm_close_socket(sock);
  }
  closeCv.notify_all();
}
