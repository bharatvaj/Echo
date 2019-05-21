#include <iostream>
#include <csignal>

#include <fmt/format.h>
#include <shell4cpp/Shell.hpp>

#include <echo/Chat.hpp>
#include <echo/EchoClient.hpp>

namespace echo_client {

using namespace echo;
using namespace std;
using namespace shell4cpp;

EchoClient *c = nullptr;

//
Shell shell("echo> ");

// read write callbacks are queued here
// streams are given higher priority
class EchoLooper {};

void print(Chat *chat) {
  if(c == nullptr){
    cout << "message read failed" << endl;
    return;
  }
  cout << chat->from << ": " << chat->chat << endl;
}

// input only
void stream(Chat *chat) {
  fprintf(stderr, "%s", chat->chat);
}

void send_error_handle(Chat *str) {
  if(str == nullptr){
    cout << "Error in sending chat, try again later" << endl;
    return;
  }
}

void msgSent(Chat *) {}

void inited(Echo *e) {
  if(e == nullptr){
    cout << "Cannot initialize echo" << endl;
    return; //report error
  }
  // EchoClient &echo = *(EchoClient *)e;
  cout << "Initialization done" << endl;
  // loading done
  // setServers
  // load list of servers from file

  //Chat *chat;

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

void finished(Echo *){
  //disconnected, now clear up
}

void start(Operation op, vector<string> args){
  if(args.size() == 0){
    cout << "Start requires userid" << endl;
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


void server(Operation op, vector<string> args){
  if(args.size() == 0){
    cout << "Enter ip address of server" << endl;
    return;
  }
  c->setServer(args[0]);
}

void sendMessage(Operation o, vector<string> args){
  if(args.size() < 2){
    cout << "Not enough arguments" << endl;
    return;
  }
  if(c == nullptr){
    cout << "Echo not initialized, start echo first" << endl;
    return;
  }
  Chat *chat = createChat(c->getUserId().c_str(), args[0].c_str(), args[1].c_str(), (uint32_t)args[1].size(), false);
  c->send(chat);
}
void help(Operation o, vector<string> args){
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
    Operation *op = ops[msg];
    if (op == nullptr)
      continue;
    shell.println(op->usage);
  }
  }
}

void stop(Operation o, vector<string> args){
  EchoClient::getInstance()->close();
}

void exit_handler(int) {
  cout << "Shutting down Echo" << endl;
  EchoClient::getInstance()->close();
}



vector<Operation *> getOperations(){
  vector<Operation *> o;
  o.push_back(new Operation("start", "starts echo client", start, "start <userId>"));
  o.push_back(new Operation("server", "set the server location", server, "server <ip>"));
  o.push_back(new Operation("send", "sends message to a user", sendMessage, "send <to> <message>"));
  o.push_back(new Operation("stop", "stops the connection", stop, "stop"));
  o.push_back(new Operation("help", "prints help message", help, "help [operation]"));
  //
  return o;
}

}

int main(int, char *) {
  signal(SIGINT, echo_client::exit_handler);
  // clog_enable();
  comm_init();

  echo_client::c = echo::EchoClient::getInstance();

  for(auto o : echo_client::getOperations()){
    echo_client::shell.addOperation(o);
  }
  new std::thread([=]{
    echo_client::shell.start();
    // c->close();
  });

  if(echo_client::c == nullptr) return -1;
  int status = echo_client::c->waitForClose();
  return status;
}
