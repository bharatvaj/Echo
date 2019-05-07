#ifndef _ECHO_CHAT_H
#define _ECHO_CHAT_H
#include <crossguid/guid.hpp>
#include <cstddef>
#include <iostream>

namespace echo {
typedef struct {
  char id[8];
  char from[8];
  char to[8];
  bool isStream;
  char *chat;
  int chatLen;
} Chat;

Chat *createChat(const char *, const char *, const char *, int, bool = false);
} // namespace echo
#endif
