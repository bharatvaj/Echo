#include <clog/clog.h>
#include <echo/Echo.hpp>
#include <echo/EchoWriter.hpp>
#include <comm.h>
#include <crosssocket.h>

// static const char *TAG = "Echo";

echo::Echo::~Echo(){
  clog_enable();
}

std::string echo::Echo::getUserId() {
  return userId;
}

void echo::Echo::setUserId(std::string uid) {
  this->userId = uid;
}

void echo::Echo::setInitCallback(EchoCallback callback){
  this->initCallback = callback;
}

void echo::Echo::setFinishCallback(EchoCallback callback){
  this->finishCallback = callback;
}

void echo::Echo::send(Chat *chat)
{
  if (chat == nullptr)
  {
    //sendErrorCallback(chat);
    fireEvent(EchoEvent::SEND_ERR, chat);
    return;
  }
  if(!EchoWriter::getInstance()->write(getServerSocket(), chat)){
    fireEvent(EchoEvent::SEND_ERR, chat);
    // sendErrorCallback(chat);
    return;
  }
  fireEvent(EchoEvent::SENT, chat);
  // sentCallback(chat);
}

void echo::Echo::block(std::string str)
{
  // TODO
}

void echo::Echo::unblock(std::string uid)
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
  delete this;
  return status;
}

void echo::Echo::close()
{
  if (sock != xs_ERROR)
  {
    comm_close_socket(sock);
  }
  closeCv.notify_all();
}
