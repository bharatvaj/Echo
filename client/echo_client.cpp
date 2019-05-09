#include <clog/clog.h>
#include <csignal>
#include <echo/Chat.hpp>
#include <echo/EchoClient.hpp>
#include <iostream>
#include <unistd.h>

#include <clog/clog.h>

using namespace echo;

static const char *TAG = "echo_client";

// read write callbacks are queued here
// streams are given higher priority
class EchoLooper {};

void print(Chat *c) {
  if(c == nullptr){
    clog_e(TAG, "message read failed");
    return;
  }
  std::cout << c->from << ": " << c->chat << std::endl;
}

// input only
void stream(Chat *chat) {
  /*****do all this in EchoReader*******/
  char *str = new char(chat->chatLen + 1);
  str[chat->chatLen] = '\0';
  /************/
  fprintf(stderr, "%s", str);
}

void send_error_handle(Chat *str) {
  if(str == nullptr){
    std::cout << "Error in sending chat, try again later" << std::endl;
    return;
  }
}

void msgSent(Chat *chat) {}

void inited(Echo *e) {
  if(e == nullptr){
    clog_e(TAG, "Cannot initialize echo");;
    return; //report error
  }
  EchoClient &echo = *(EchoClient *)e;
  clog_i(TAG, "Initialization done");
  // loading done
  // setServers
  // load list of servers from file

  Chat *chat;

  //   chat = createChat(false, "kkk", 200);              // file
  //   chat = createChat(false, "AUD_VID_STREAM", 200); // file
  // echo.send("fakerider1", chat); // send - burst fire

  // blocking
  // no callback from fakerider1
  // echo.block("fakerider1");
  // array of str - blocked users
  // echo.getBlockedUsers();
  // yes callback from fakerider1
  // echo.unblock("fakerider1");

  // clean resources, close all stream, disconnect
  // echo.close();
}

void finished(Echo *e){
  //
}

void exit_handler(int sig) {
  clog_i(TAG, "Shutting down Echo");
  EchoClient::getInstance()->close();
}

int main(int argc, char *argv[]) {
  signal(SIGINT, exit_handler);
  clog_enable();

  // ui classes here
  // ui loading

  EchoClient *echo = EchoClient::getInstance();

  echo->setServer("localhost");

  echo->setUser("ghostrider001"); // automatic ssh key authorization

  echo->setInitCallback(inited);
  echo->setFinishCallback(finished);

  // register read and stream callback
  echo->setReadCallback(print);
  echo->setStreamCallback(stream);

  // called when cannot send a chat
  echo->setSendErrorCallback(send_error_handle);
  echo->setSentCallback(msgSent);
  echo->initialize();

  return echo->waitForClose();
}
