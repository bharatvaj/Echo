#ifndef _ECHO_WRITER_H
#define _ECHO_WRITER_H
#include <echo/Chat.hpp>
#include <iostream>
#include <crosssocket.h>
namespace echo {
class EchoWriter {

  static EchoWriter *instance;

  int READ_SZ = 128;
  int deadLock = READ_SZ / 10;

  int write(xs_SOCKET sock, char *buffer, uint32_t bufferLen, int i = 0);


public:
  static EchoWriter *getInstance() ;
  bool write(xs_SOCKET sock, Chat *chat);
};
} // namespace echo
#endif
