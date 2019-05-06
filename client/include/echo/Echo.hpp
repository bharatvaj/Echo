#ifndef _ECHO_H
#define _ECHO_H
#include <crosssocket.h>
#include <echo/Chat.hpp>
#include <echo/EchoReader.hpp>
#include <functional>
#include <map>
#include <string>
#include <thread>
#include <vector>
#include <thread>
#include<condition_variable>
#include <mutex>
namespace echo {
class Echo {

  typedef void (*ChatCallback)(Chat *);
  typedef void (*InitCallback)(Echo &);
  std::string server;
  int status = 0;

  static xs_SOCKET sock;

  static Echo *instance;
  std::mutex closeLck;
  std::condition_variable closeCv;
  bool shouldClose = false;
  ChatCallback readCallback;
  ChatCallback streamCallback;
  ChatCallback sendErrorCallback;
  ChatCallback sentCallback;

  xs_SOCKET getServerSocket();

public:
  static Echo *getInstance();
  /*blocking call*/
  void initialize(InitCallback);

  /*set handles*/
  void setReadCallback(ChatCallback);
  void setStreamCallback(ChatCallback);
  void setSendErrorCallback(ChatCallback);
  void setSentCallback(ChatCallback);

  void setServer(std::string);

  void setUser(std::string userId);

  void send(std::string id, Chat *);
  void block(std::string id);
  std::vector<std::string> getBlockedUsers();
  void unblock(std::string id);
  int waitForClose();
  void close();
};
} // namespace echo
#endif