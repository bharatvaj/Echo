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
#include <echo/EchoEvent.hpp>
#include <em/EventManager.hpp>

#define ECHO_DEFAULT_PORT 8000

namespace echo {

class Echo : public em::EventManager<EchoEvent, Chat *> {

protected:
  typedef void (*EchoCallback)(Echo *);
  std::string userId;
  int status = 0;

  xs_SOCKET sock = SOCKET_ERROR;

  std::mutex closeLck;
  std::condition_variable closeCv;

  EchoCallback initCallback;
  EchoCallback finishCallback;

  virtual xs_SOCKET getServerSocket() = 0;

public:
  /*blocking call*/
  virtual void initialize() = 0;

  std::string getUserId();
  void setUserId(std::string);

  /*set echo lifecycle handles*/
  void setInitCallback(EchoCallback);
  void setFinishCallback(EchoCallback);

  void send(Chat *);
  void block(std::string id);
  std::vector<std::string> getBlockedUsers();
  void unblock(std::string id);
  virtual int waitForClose() final;
  virtual void close() final;
};
} // namespace echo
#endif
