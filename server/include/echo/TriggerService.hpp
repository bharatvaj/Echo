#ifndef _E_TRIGGER_SERVICE_H
#define _E_TRIGGER_SERVICE_H
#include <map>
#include <echo/Echo.hpp>
#include <echo/Coordinator.hpp>
namespace echo {
  class Coordinator;
  class TriggerService {
    std::map<Echo *, bool> instances;
    static TriggerService *instance;
    std::vector<Coordinator> v;
    //connect to DB
    TriggerService();

  public:
    static TriggerService *getInstance();
    void registerClient(echo::Coordinator *);
    void unregisterClient(echo::Coordinator *);
  };
}
#endif
