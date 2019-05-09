#include <clog/clog.h>
#include <condition_variable>
#include <csignal>
#include <echo/EchoServer.hpp>
#include <echo/Coordinator.hpp>
#include <iostream>
#include <mutex>
#include <vector>
#include <thread>
#include <map>

static const char *TAG = "echo_server";

std::mutex closeLck;
std::condition_variable closeCv;

std::map<echo::Echo *, echo::Coordinator *> instances;
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
    instance.first->close();
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
  clog_i(TAG, "Echo server instance initialized");
  echo::Coordinator *c = new echo::Coordinator(e);
  instances[e] = c;
}

void finished(echo::Echo *e){
  if(e == nullptr) return;
  echo::Coordinator *c = instances[e];
  if(c != nullptr){
    delete c; //unregisters and gets deleted
  }
  e->close(); //close socket
  delete e;
  instances.erase(e); //delete from instance map
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
    s->setInitCallback(initialized);
    s->setFinishCallback(finished);
    //a client has connected
    //non-blocking
    std::thread *t = new std::thread([](echo::EchoServer *s){
      if(s != nullptr){
        s->initialize();
      }
    }, s);
    instanceThreads.push_back(t);
    if(sock == SOCKET_ERROR) break;
  }
  return waitForClose();
}
