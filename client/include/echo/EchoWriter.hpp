#ifndef _ECHO_WRITER_H
#define _ECHO_WRITER_H
#include <echo/Chat.hpp>
#include <echo/Common.hpp>
#include <iostream>
namespace echo {
class EchoWriter {

  static EchoWriter *instance;

  int READ_SZ = 128;
  int deadLock = READ_SZ / 10;

  int write(xs_SOCKET sock, const char *buffer, int bufferLen, int i = 0) {
    if (i > deadLock)
      return -1;
    int wroteBytes = ::write(1, buffer, bufferLen);
    if (wroteBytes < bufferLen) {
      /*d - delta*/
      int d = bufferLen - wroteBytes;
      write(sock, buffer + wroteBytes, d, i++);
    }
    return 0;
  }


public:
  static EchoWriter *getInstance() {
    if (instance == nullptr) {
      instance = new EchoWriter();
    }
    return instance;
  }

  bool write(xs_SOCKET sock, Chat *chat) {
    if (chat == nullptr)
      return false;
    if (sock == SOCKET_ERROR)
      return false;
    if (chat->chat == nullptr)
      return false;
    int status = write(sock, (char *)chat->chat, chat->chatLength);
    return status == 0;
  }
};
EchoWriter *EchoWriter::instance = nullptr;
} // namespace echo
#endif