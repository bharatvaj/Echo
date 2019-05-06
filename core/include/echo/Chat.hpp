#ifndef _ECHO_CHAT_H
#define _ECHO_CHAT_H
#include <crossguid/guid.hpp>
#include <iostream>

namespace echo {
typedef struct {
  std::string id;
  std::string from;
  std::string to;
  bool isStream;
  int chatLength;
  const void *chat;
} Chat;

static Chat *createChat(std::string, void *, int, bool = false);
} // namespace echo
#endif