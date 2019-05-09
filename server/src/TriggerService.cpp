#include <echo/TriggerService.hpp>
#include <echo/Coordinator.hpp>

echo::TriggerService *echo::TriggerService::instance = nullptr;

echo::TriggerService::TriggerService(){
  //
}

echo::TriggerService *echo::TriggerService::getInstance(){
  if(instance == nullptr){
    instance = new TriggerService();
  }
  return instance;
}

void echo::TriggerService::registerClient(echo::Coordinator *c){
  //
}

void echo::TriggerService::unregisterClient(echo::Coordinator *c){
  //
}
