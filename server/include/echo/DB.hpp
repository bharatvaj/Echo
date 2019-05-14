#ifndef _ECHO_DB_H
#define _ECHO_DB_H
#include <echo/Chat.hpp>
#include <sqlite3.h>
#include <mutex>
#include <vector>
namespace echo {
  class DB {
    DB();
    ~DB();
    static DB *instance;
    static sqlite3 *db;
    std::mutex dbLock;
  public:
    static DB *getInstance();
    static void deleteInstance();
    //
    void enqueueChat(echo::Chat *);
    std::vector<Chat *> getChatsFor(const char *);
    void deleteChat(Chat *);
  };
}
#endif
