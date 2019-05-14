#include <echo/TriggerService.hpp>
#include <algorithm>

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

void echo::TriggerService::registerClient(echo::Echo *c){
  //add to active intances
  instances.push_back(c);
}

void echo::TriggerService::unregisterClient(echo::Echo *c){
  //remove from active instances
  instances.erase(std::remove(instances.begin(), instances.end(), c), instances.end());
}

echo::Echo *echo::TriggerService::isOnline(const char *userId){
  for(auto v : instances){
    if(userId == v->getUserId()){
      //online
      return v;
    }
  }
  return nullptr;
}
