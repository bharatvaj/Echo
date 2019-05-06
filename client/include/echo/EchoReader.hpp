#ifndef _ECHO_READER_H
#define _ECHO_READER_H
#include <comm.h>
#include <echo/Chat.hpp>
#include <echo/Common.hpp>
#include <iostream>
#include <unistd.h>
namespace echo {
class EchoReader {
  static EchoReader *instance;
  int READ_SZ = 128;
  int deadLock = READ_SZ / 10;

  int read(int sock, char *buffer, int bufferLen, int i = 0) {
    if (i > deadLock)
      return -1;
    int readBytes = ::read(1, buffer, bufferLen);
    if (readBytes < bufferLen) {
      /*d - delta*/
      int d = bufferLen - readBytes;
      read(sock, buffer + readBytes, d, i++);
    }
    return 0;
  }

public:
  static EchoReader *getInstance() {
    if (instance == nullptr) {
      instance = new EchoReader();
    }
    return instance;
  }
  Chat *read(xs_SOCKET sock) {
    if (sock == SOCKET_ERROR)
      return nullptr;
    void *buffer = (void *)malloc(READ_SZ);
    int readStatus = read(sock, (char *)buffer, READ_SZ); // chat size
    if (readStatus == -1) {
      return nullptr;
    }
    Chat *chat = (Chat *)buffer;
    if (chat->to == echo::Common::userId) {
      return chat;
    }
    return nullptr;
  }
};
} // namespace echo
#endif