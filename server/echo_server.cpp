#include <clog/clog.h>
#include <condition_variable>
#include <csignal>
#include <echo/EchoServer.hpp>
#include <iostream>
#include <mutex>
#include <vector>

static const char *TAG = "echo_server";

std::mutex closeLck;
std::condition_variable closeCv;

std::vector<echo::EchoServer *> instances;

void exit_handler(int sig) {
  clog_i(TAG, "Shutting down echo server");
  // close all server instances
  for (auto instance : instances) {
    instance->close();
  }
  closeCv.notify_all();
}

int waitForClose() {
  std::unique_lock<std::mutex> lck(closeLck);
  closeCv.wait(lck);
  return 0;
}

void initialized(echo::Echo *e) {
  if(e == nullptr){
    clog_e(TAG, "Cannot initialize echo");
    return;
  }
  echo::EchoServer &echo = *(echo::EchoServer *)e;
  clog_i(TAG, "Echo server instance initialized");
}

int main(int argc, char *argv[]) {
  signal(SIGINT, exit_handler);
  clog_enable();
  while (1) {
    xs_SOCKET sock = comm_start_server(ECHO_DEFAULT_PORT);
    if (sock == SOCKET_ERROR) {
      clog_f(TAG, "Cannot start server");
      return -1;
    }
    echo::EchoServer *s = echo::EchoServer::getInstance(sock);
    instances.push_back(s);
    s->initialize(initialized);
  }
  return waitForClose();
}
