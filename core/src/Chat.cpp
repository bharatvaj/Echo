#include <echo/Chat.hpp>
#include <cstring>
#include <crossguid/guid.hpp>

echo::Chat *echo::createChat(const char *from, const char *to, const char *chat, uint32_t chatLen, bool isStream) {
  Chat *c = new Chat();
  memset(c, '\0', sizeof(Chat));
  memcpy((void *)c->from, from, 7);
  memcpy((void *)c->to, to, 7);
  c->isStream = isStream;
  //Generate 8 byte length guid
  memcpy(c->id, xg::newGuid().str().c_str(), 7);
  c->chatLen = chatLen;
  memcpy(c->chat, chat, chatLen);
  return c;
}
