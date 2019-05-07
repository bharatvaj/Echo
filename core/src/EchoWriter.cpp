#include <echo/EchoWriter.hpp>
#include <echo/Chat.hpp>
#include <crosssocket.h>

echo::EchoWriter *echo::EchoWriter::instance = nullptr;

  int echo::EchoWriter::write(xs_SOCKET sock, char *buffer, int bufferLen, int i) {
    if (i > deadLock)
      return -1;
    int wroteBytes = ::write(sock, buffer, bufferLen);
    if (wroteBytes < bufferLen) {
      /*d - delta*/
      int d = bufferLen - wroteBytes;
      write(sock, buffer + wroteBytes, d, i++);
    }
    return 0;
  }

  echo::EchoWriter *echo::EchoWriter::getInstance() {
    if (instance == nullptr) {
      instance = new EchoWriter();
    }
    return instance;
  }

  bool echo::EchoWriter::write(xs_SOCKET sock, echo::Chat *chat) {
    if (chat == nullptr)
      return false;
    if (sock == SOCKET_ERROR)
      return false;
    if (chat->chat == nullptr)
      return false;
    int status = write(sock, (char *)chat, sizeof(Chat) + chat->chatLen);
    return status == 0;
  }
