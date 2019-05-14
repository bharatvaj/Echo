#include <clog/clog.h>
#include <condition_variable>
#include <csignal>
#include <echo/EchoServer.hpp>
#include <echo/TriggerService.hpp>
#include <iostream>
#include <mutex>
#include <vector>
#include <thread>
#include <algorithm>
#include <map>

static const char *TAG = "echo_server";

echo::TriggerService *triggerService = nullptr;

std::mutex closeLck;
std::mutex instanceVectorLock;
std::condition_variable closeCv;

std::vector<echo::Echo *> instances;
std::vector<std::thread *> instanceThreads;
xs_SOCKET sock = SOCKET_ERROR;

void exit_handler(int sig) {
  clog_i(TAG, "Shutting down echo server");
  // close all server instances
  if(sock != SOCKET_ERROR){
    comm_close_socket(sock);
  }
  sock = SOCKET_ERROR;
  if(triggerService != nullptr){
    delete triggerService;
  }
  closeCv.notify_all();
}

int waitForClose() {
  std::unique_lock<std::mutex> lck(closeLck);
  closeCv.wait(lck);

  for (auto instance : instances) {
      instance->close();
  }
  instances.clear();
  for(auto instance : instanceThreads){
      instance->join();
  }
  instanceThreads.clear();
  return 0;
}

void addToInstance(echo::Echo *e){
  instanceVectorLock.lock();
  instances.push_back(e);
  instanceVectorLock.unlock();
}

void removeFromInstance(echo::Echo *e){
  instanceVectorLock.lock();
  std::vector<echo::Echo *>::iterator pos = std::find(instances.begin(), instances.end(), e);
  if (pos != instances.end()){
    instances.erase(pos);
  }
  instanceVectorLock.unlock();
}

void initialized(echo::Echo *e) {
  if(e == nullptr){
    clog_e(TAG, "Cannot initialize echo");
    return;
  }
  clog_i(TAG, "Echo server instance initialized");
  // addToInstance(e);
  //register
  triggerService->registerClient(e);
}

void finished(echo::Echo *e){
  if(e == nullptr) return;
  triggerService->unregisterClient(e);
  removeFromInstance(e);
  e->close(); //close socket
}


int main(int argc, char *argv[]) {
  signal(SIGINT, exit_handler);
  clog_enable();
  //probably blocking
  triggerService = echo::TriggerService::getInstance();
    sock = comm_start_server(ECHO_DEFAULT_PORT);
    if (sock == SOCKET_ERROR) {
      clog_f(TAG, "Cannot start server");
      return -1;
    }

  while (1) {
    if (sock == SOCKET_ERROR) {
      clog_f(TAG, "Cannot start server");
      break;
    }
    //waiting for client to connect
    //blocking
    echo::EchoServer *s = echo::EchoServer::getInstance();
    s->setInitCallback(initialized);
    s->setFinishCallback(finished);

    // echo::Chat *chat = echo::createChat("ghostrider001", "server", "message", 7);
    //a client has connected
    //non-blocking
    addToInstance(s);
    std::thread *t = new std::thread([](echo::EchoServer *s){
      if(s != nullptr){
        s->initialize();
      }
    }, s);
    instanceThreads.push_back(t);
  }
  return waitForClose();
}
