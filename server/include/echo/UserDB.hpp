#ifndef _ECHO_USER_DB_H
#define _ECHO_USER_DB_H
#include <sqlite3.h>
#include <mutex>
#include <vector>
namespace echo {
  class UserDB {
    UserDB();
    ~UserDB();
    static UserDB *instance;
    static sqlite3 *db;
    std::mutex dbLock;
  public:
    static UserDB *getInstance();
    static void deleteInstance();
    //
    bool idExists(const char *);
    bool keyChecks(const char *, const char *, int);
    void saveKey(const char *, const char *, int);
  };
}
#endif
