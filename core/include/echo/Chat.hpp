#ifndef _ECHO_CHAT_H
#define _ECHO_CHAT_H
#include <crossguid/guid.hpp>
#include <cstddef>
#include <iostream>
/*
| id       	| 8   	|
| from     	| 8   	|
| to       	| 8   	|
| isStream 	| 1   	|
| chatLen  	| 4   	|
| chat     	| 128 	|
| total     | 157   |
*/
namespace echo {
typedef struct{
  char id[8];
  char from[8];
  char to[8];
  bool isStream;
  char chat[128];
  uint32_t chatLen;
} Chat;

Chat *createChat(const char *, const char *, const char *, uint32_t, bool = false);
} // namespace echo
#endif
