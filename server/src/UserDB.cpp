#include <echo/UserDB.hpp>
#include <sqlite3.h>
#include <clog/clog.h>
#include <fmt/format.h>

static const char *TAG = "DB";

echo::UserDB *echo::UserDB::instance = nullptr;
sqlite3 *echo::UserDB::db = nullptr;

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
   int i;
   for(i = 0; i<argc; i++) {
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}

echo::UserDB::UserDB(){
  clog_i(TAG, "Sqlite version: %s", sqlite3_libversion());
  //open create table chats (id text, from text, to text, chat text)
    // int rc = sqlite3_open("db.sqlite3", &db);
    // if (rc != SQLITE_OK) {
    //     clog_f(TAG, "Cannot open database: %s\n", sqlite3_errmsg(db));
    //     sqlite3_close(db);
    //     return;
    // }
    //create table
    char *zErrMsg = 0;
    std::string sql;
    // std::string sql = "CREATE TABLE Chat (\
    // id	TEXT,\
    // s	TEXT,\
    // r	TEXT,\
    // chat	TEXT,\
    // chatLen	INTEGER,\
    // isStream	INTEGER,\
    // PRIMARY KEY(\"id\")\
    // );";
    // rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);
    // if( rc != SQLITE_OK ){
    //     fprintf(stderr, "SQL error: %s\n", zErrMsg);
    //     sqlite3_free(zErrMsg);
    //  }
}

echo::UserDB::~UserDB(){
  sqlite3_close(db);
}

echo::UserDB *echo::UserDB::getInstance(){
  if(instance == nullptr){
    instance = new UserDB();
  }
  return instance;
}

void echo::UserDB::deleteInstance(){
  if(instance != nullptr){
    //close connection to DB
    delete instance;
  }
}

bool echo::UserDB::idExists(const char *userId) {
  return true;
}
bool echo::UserDB::keyChecks(const char *userId, const char *chat, int chatLen) {
  return true;
}

void echo::UserDB::saveKey(const char *userId, const char *chat, int chatLen) {

}
