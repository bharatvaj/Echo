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

//echo must not be null
echo::Coordinator::Coordinator(echo::Echo *echo) : echo(echo){
  echo->setSentCallback(sent);
  echo->setSendErrorCallback(sendError);
  echo->setReadCallback(readCallback);
  echo->setStreamCallback(streamCallback);
  echo::Chat *chat = echo::createChat("ghostrider001", "server", "message", 7);
  //register
  TriggerService::getInstance()->registerClient(this);
  echo->send(chat); //delete
}

echo::Coordinator::~Coordinator(){
  TriggerService::getInstance()->unregisterClient(this);
}
