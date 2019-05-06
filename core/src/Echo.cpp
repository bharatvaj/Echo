#include <clog/clog.h>
#include <echo/Echo.hpp>
#include <echo/EchoWriter.hpp>

static const char *TAG = "Echo";

xs_SOCKET echo::Echo::sock = SOCKET_ERROR;

xs_SOCKET echo::Echo::getServerSocket()
{
  return SOCKET_ERROR;
}

void echo::Echo::initialize(echo::Echo::InitCallback initCallback)
{
  //
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

void echo::Echo::setUser(std::string userId) { this->userId = userId; }

void echo::Echo::send(std::string id, Chat *chat)
{
  if (chat == nullptr)
  {
    // if (sendErrorCallback != nullptr)
    sendErrorCallback(chat);
    return;
  }
  EchoWriter::getInstance()->write(getServerSocket(), chat);
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
  return std::vector<std::string>();
}

int echo::Echo::waitForClose()
{
  std::unique_lock<std::mutex> lck(closeLck);
  closeCv.wait(lck, [=] { return shouldClose; });
  return status;
}

void echo::Echo::close()
{
  if (sock != SOCKET_ERROR)
  {
    comm_close_socket(sock);
  }
  shouldClose = true;
  closeCv.notify_all();
}