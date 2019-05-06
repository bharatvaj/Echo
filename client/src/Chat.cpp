#include <echo/Chat.hpp>

echo::Chat *echo::createChat(void *chat, int len, bool isStream) {
  // if user is not loaded, don't create chat
  if (echo::Common::userId.empty())
    return nullptr;
  Chat *c = new Chat();
  c->isStream = isStream;
  c->id = xg::newGuid().str();
  c->chatLength = len;
  c->chat = chat;
  return c;
}