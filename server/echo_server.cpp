#include <clog/clog.h>
#include <condition_variable>
#include <csignal>
#include <echo/EchoServer.hpp>
#include <iostream>
#include <mutex>

static const char *TAG = "echo_server";

std::mutex closeLck;
std::condition_variable closeCv;
bool shouldClose = false;

void exit_handler(int sig) {
  clog_i(TAG, "Shutting down echo server");
  shouldClose = true;
  closeCv.notify_all();
}

int waitForClose() {
  std::unique_lock<std::mutex> lck(closeLck);
  closeCv.wait(lck, [=] { return shouldClose; });
  return 0;
}

void initialized(echo::Echo &e){
  
}

int main(int argc, char *argv[]) {
  signal(SIGINT, exit_handler);
  clog_enable();
  xs_SOCKET sock = comm_start_server(ECHO_DEFAULT_PORT);
  if(sock == SOCKET_ERROR){
    clog_f(TAG, "Cannot start server");
    return -1;
  }
  while(1){
    echo::EchoServer *s = echo::EchoServer::getInstance(sock);
    s->initialize(initialized);
  }
  return waitForClose();
}
