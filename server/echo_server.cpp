#include <clog/clog.h>
#include <condition_variable>
#include <csignal>
#include <echo/EchoServer.hpp>
#include <echo/Coordinator.hpp>
#include <iostream>
#include <mutex>
#include <vector>
#include <thread>

static const char *TAG = "echo_server";

std::mutex closeLck;
std::condition_variable closeCv;

std::vector<echo::EchoServer *> instances;
std::vector<std::thread *> instanceThreads;
xs_SOCKET sock = SOCKET_ERROR;

void exit_handler(int sig) {
  clog_i(TAG, "Shutting down echo server");
  // close all server instances
  if(sock != SOCKET_ERROR){
    comm_close_socket(sock);
  }
  sock = SOCKET_ERROR;
  closeCv.notify_all();
}

int waitForClose() {
  std::unique_lock<std::mutex> lck(closeLck);
  closeCv.wait(lck);

  for (auto instance : instances) {
    instance->close();
  }
  for(auto instance : instanceThreads){
    instance->join();
  }
  return 0;
}

void initialized(echo::Echo *e) {
  if(e == nullptr){
    clog_e(TAG, "Cannot initialize echo");
    return;
  }
  echo::EchoServer &echo = *(echo::EchoServer *)e;
  clog_i(TAG, "Echo server instance initialized");

  echo::Coordinator *c = new echo::Coordinator(e);
  //dont exit until connection closes
  // echo.onQuit()
}

int main(int argc, char *argv[]) {
  signal(SIGINT, exit_handler);
  clog_enable();
    sock = comm_start_server(ECHO_DEFAULT_PORT);
    if (sock == SOCKET_ERROR) {
      clog_f(TAG, "Cannot start server");
      return -1;
    }

  while (1) {
    //waiting for client to connect
    //blocking
    echo::EchoServer *s = echo::EchoServer::getInstance();
    //a client has connected
    //non-blocking
    instances.push_back(s);
    std::thread *t = new std::thread([](echo::EchoServer *s){
      if(s != nullptr){
        s->initialize(initialized);
      }
    }, s);
    instanceThreads.push_back(t);
    if(sock == SOCKET_ERROR) break;
  }
  return waitForClose();
}
