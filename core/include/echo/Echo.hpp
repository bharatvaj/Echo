#ifndef _ECHO_H
#define _ECHO_H
#include <condition_variable>
#include <echo/Chat.hpp>
#include <echo/EchoReader.hpp>
#include <functional>
#include <map>
#include <mutex>
#include <string>
#include <thread>
#include <vector>
#include <crosssocket.h>

#define ECHO_DEFAULT_PORT 8000

namespace echo {
class Echo {

protected:
  typedef void (*ChatCallback)(Chat *);
  typedef void (*InitCallback)(Echo *);
  int status = 0;

  xs_SOCKET sock = SOCKET_ERROR;

  std::mutex closeLck;
  std::condition_variable closeCv;
  ChatCallback readCallback;
  ChatCallback streamCallback;
  ChatCallback sendErrorCallback;
  ChatCallback sentCallback;

  virtual xs_SOCKET getServerSocket() = 0;

public:
  /*blocking call*/
  virtual void initialize(InitCallback) = 0;

  /*set handles*/
  void setReadCallback(ChatCallback);
  void setStreamCallback(ChatCallback);
  void setSendErrorCallback(ChatCallback);
  void setSentCallback(ChatCallback);

  void send(Chat *);
  void block(std::string id);
  std::vector<std::string> getBlockedUsers();
  void unblock(std::string id);
  virtual int waitForClose() final;
  virtual void close() final;
};
} // namespace echo
#endif
