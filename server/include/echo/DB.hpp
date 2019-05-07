#ifndef _ECHO_DB_H
#define _ECHO_DB_H
#include <echo/Chat.hpp>
namespace echo {
  class DB {
    static DB *instance;
  public:
    static DB *getInstance();
    bool idExists(const char *);
    bool keyChecks(const char *, const char *, int);
    void saveKey(const char *, const char *, int);
  };
}
#endif
