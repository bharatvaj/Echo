#include <iostream>
#include <crosssocket.h>
#include <comm.h>

#include <echo/EchoWriter.hpp>
#include <echo/EchoReader.hpp>
#include <echo/Chat.hpp>

using namespace std;
using namespace echo;

#define PORT 5000

int startClient(){

    xs_SOCKET client = comm_connect_server("localhost", PORT);
    if(client == SOCKET_ERROR){
      cout << "Cannot connect" << endl;
      return -1;
    }
    Chat *c = EchoReader::getInstance()->read(client);

    cout << c->id << endl;
    cout << c->from << endl;
    cout << c->to << endl;
    cout << c->isStream << endl;
    cout << c->chat << endl;
    cout << c->chatLen << endl;

    comm_close_socket(client);
    return 0;
}

int startServer(){
    xs_SOCKET sock = comm_start_server(PORT);
    if(sock == SOCKET_ERROR){
      cout << "Cannot start server" << endl;
      return -1;
    }

    Chat *chat = createChat("gh", "fakerider1", "bigmessage\n", 7);

    if(!EchoWriter::getInstance()->write(sock, chat)){
      cout << "Cannot writer chat" << endl;
      return -1;
    }

    comm_close_socket(sock);
    return 0;
}


int main(int argc, char *argv[]){
  return argc > 1 ? startClient() : startServer();
}
