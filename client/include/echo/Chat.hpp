#ifndef _ECHO_CHAT_H
#define _ECHO_CHAT_H
#include <crossguid/guid.hpp>
#include <iostream>

#include <echo/Common.hpp>

namespace echo {
typedef struct {
  std::string id;
  std::string from;
  std::string to;
  bool isStream;
  int chatLength;
  const void *chat;
} Chat;

static Chat *createChat(void *chat, int len, bool isStream = false);
} // namespace echo
#endif