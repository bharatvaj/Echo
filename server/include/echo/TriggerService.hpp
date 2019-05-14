#ifndef _E_TRIGGER_SERVICE_H
#define _E_TRIGGER_SERVICE_H
#include <map>
#include <echo/Echo.hpp>
namespace echo {
  class Coordinator;
  enum class TriggerServiceEvent {
    NEW_CHAT
  };
  class TriggerService : public em::EventManager<TriggerServiceEvent, Chat *> {
    //echoInstances or registeredClients
    std::vector<Echo *> instances;
    static TriggerService *instance;
    //connect to DB
    TriggerService();

  public:
    static TriggerService *getInstance();
    void registerClient(echo::Echo *);
    void unregisterClient(echo::Echo *);
    Echo *isOnline(const char *);
  };
}
#endif
