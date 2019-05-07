#include <echo/DB.hpp>

echo::DB *echo::DB::instance = nullptr;

echo::DB *echo::DB::getInstance(){
  if(instance == nullptr){
    instance = new DB();
  }
  return instance;
}


bool echo::DB::idExists(const char *userId) {
  return true;
}
bool echo::DB::keyChecks(const char *userId, const char *chat, int chatLen) {
  return true;
}

void echo::DB::saveKey(const char *userId, const char *chat, int chatLen) {

}
