#ifndef _ECHO_READER_H
#define _ECHO_READER_H
#include <comm.h>
#include <echo/Chat.hpp>
#include <iostream>
#include <unistd.h>
namespace echo {
class EchoReader {
  static EchoReader *instance;
  int READ_SZ = 128;
  int deadLock = READ_SZ / 10;

  int read(int sock, char *buffer, int bufferLen, int i = 0);

public:
  static EchoReader *getInstance() ;
  Chat *read(xs_SOCKET sock);
};
} // namespace echo
#endif