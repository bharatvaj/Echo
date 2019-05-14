#include <clog/clog.h>
#include <csignal>
#include <echo/Chat.hpp>
#include <echo/EchoClient.hpp>
#include <iostream>
#include <unistd.h>
#include <fmt/format.h>

#include <clog/clog.h>
#include <shell4cpp/Shell.hpp>

using namespace echo;

static const char *TAG = "echo_client";

echo::EchoClient *c = nullptr;

//
shell4cpp::Shell shell("echo> ");

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
  //disconnected, now clear up
}

void start(shell4cpp::Operation op, std::vector<std::string> args){
  c->setServer("localhost");

  if(args.size() == 0){
    std::cout << "Start requires userid" << std::endl;
    return;
  }

  c->setUserId(args[0]);

  c->setInitCallback(inited);
  c->setFinishCallback(finished);

  // register read and stream callback
  c->on(EchoEvent::READ, print);
  c->on(EchoEvent::STREAM, stream);

  // called when cannot send a chat
  c->on(EchoEvent::SEND_ERR, send_error_handle);
  c->on(EchoEvent::SENT, msgSent);
  c->initialize();
}

void sendMessage(shell4cpp::Operation o, std::vector<std::string> args){
  if(args.size() < 2){
    std::cout << "Not enough arguments" << std::endl;
    return;
  }
  if(c == nullptr){
    std::cout << "Echo not initialized, start echo first" << std::endl;
    return;
  }
  echo::Chat *chat = echo::createChat(c->getUserId().c_str(), args[0].c_str(), args[1].c_str(), args[1].size(), false);
  c->send(chat);
}
void help(shell4cpp::Operation o, std::vector<std::string> args){
  auto ops = shell.getOperations();
  if (args.size() == 0) {
    for (auto op : ops) {
      if (op.second == nullptr)
        continue;
      shell.println(fmt::format("\u001b[33m{}\u001b[0m\t- {}", op.first,
                                op.second->description));
    }
    return;
  }
  for (auto msg : args) {
  if (ops.find(msg) != ops.end()) {
    shell4cpp::Operation *op = ops[msg];
    if (op == nullptr)
      continue;
    shell.println(op->usage);
  }
  }
}

void stop(shell4cpp::Operation o, std::vector<std::string> args){
  EchoClient::getInstance()->close();
}

void exit_handler(int sig) {
  clog_i(TAG, "Shutting down Echo");
  EchoClient::getInstance()->close();
}



std::vector<shell4cpp::Operation *> getOperations(){
  std::vector<shell4cpp::Operation *> o;
  o.push_back(new shell4cpp::Operation("start", "starts echo client", start, "start <userId> [ip][:port]"));
  o.push_back(new shell4cpp::Operation("send", "sends message to a user", sendMessage, "send <to> <message>"));
  o.push_back(new shell4cpp::Operation("stop", "stops the connection", stop, "stop"));
  o.push_back(new shell4cpp::Operation("help", "prints help message", help, "help [operation]"));
  //
  return o;
}

int main(int argc, char *argv[]) {
  signal(SIGINT, exit_handler);
  clog_enable();

  c = EchoClient::getInstance();

  for(auto o : getOperations()){
    shell.addOperation(o);
  }
  new std::thread([=]{
    shell.start();
    // c->close();
  });

  if(c == nullptr) return -1;
  int status = c->waitForClose();
  delete c;
  return status;
}
