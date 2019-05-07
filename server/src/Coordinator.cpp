#include <echo/Coordinator.hpp>


void sent(echo::Chat *chat){
  //
}

void sendError(echo::Chat *chat){
  //
}

void readCallback(echo::Chat *chat){
  //
}

void streamCallback(echo::Chat *chat){
  //
}

echo::Coordinator::Coordinator(echo::Echo *echo){
  echo->setSentCallback(sent);
  echo->setSendErrorCallback(sendError);
  echo->setReadCallback(readCallback);
  echo->setStreamCallback(streamCallback);
  echo::Chat *chat = echo::createChat("ghostrider001", "server", "message", 7);
  //delegate to coordinator
  echo->send(chat);
}
