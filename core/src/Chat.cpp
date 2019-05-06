#include <echo/Chat.hpp>

static echo::Chat *echo::createChat(std::string userId, void *chat, int len, bool isStream) {
  // if user is not loaded, don't create chat
  if (userId.empty())
    return nullptr;
  Chat *c = new Chat();
  c->isStream = isStream;
  c->id = xg::newGuid().str();
  c->chatLength = len;
  c->chat = chat;
  return c;
}