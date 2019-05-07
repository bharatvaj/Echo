#include <echo/Chat.hpp>
#include <cstddef>
#include <cstring>

echo::Chat *echo::createChat(const char *from, const char *to, const char *chat, int chatLen, bool isStream) {
  Chat *c = new Chat();
  memcpy((void *)c->from, from, 8);
  memcpy((void *)c->to, to, 8);
  c->isStream = isStream;
  //Generate 8 byte length guid
  memcpy(c->id, xg::newGuid().str().c_str(), 8);
  c->chatLen = chatLen;
  c->chat = (char *)calloc(sizeof(char), chatLen);
  memcpy(c->chat, chat, chatLen);
  return c;
}
