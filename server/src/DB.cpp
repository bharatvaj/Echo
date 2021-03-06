#include <echo/DB.hpp>
#include <sqlite3.h>
#include <clog/clog.h>
#include <fmt/format.h>

static const char *TAG = "DB";

static const char *CREATE_TABLE_QUERY = "CREATE TABLE Chat ("
"id	TEXT,"
"s	TEXT,"
"r	TEXT,"
"chat	TEXT,"
"PRIMARY KEY(\"id\")"
")";

echo::DB *echo::DB::instance = nullptr;
sqlite3 *echo::DB::db = nullptr;

static int callback(void *, int argc, char **argv, char **azColName) {
   int i;
   for(i = 0; i<argc; i++) {
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}

echo::DB::DB(){
  clog_i(TAG, "Sqlite version: %s", sqlite3_libversion());
  //open create table chats (id text, from text, to text, chat text)
    int rc = sqlite3_open("chats.db", &db);
    if (rc != SQLITE_OK) {
        clog_f(TAG, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }
    //create table
    char *zErrMsg = 0;
    std::string sql = CREATE_TABLE_QUERY;
    rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
     }
}

echo::DB::~DB(){
  sqlite3_close(db);
}

void echo::DB::enqueueChat(echo::Chat *chat){
  // dbLock.lock();
  // add to chat to db
  std::string sql = fmt::format("insert into Chat values(\"{}\",\"{}\",\"{}\",\"{}\")", chat->id, chat->from, chat->to, chat->chat);
  // create table
  char *zErrMsg = 0;
  int rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);
  if( rc != SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   }
  // dbLock.unlock();
}

std::vector<echo::Chat *> echo::DB::getChatsFor(const char *userId){
  std::vector<Chat *> chats;
  std::string sql = fmt::format("select * from Chat where r=\"{}\"", userId);
  sqlite3_stmt *stmt;
  int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);
  if (rc != SQLITE_OK) {
    clog_f(TAG, "error: %s", sqlite3_errmsg(db));
    return chats;
  }
  while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
    const char *id = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
    const char *s = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
    const char *r = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
    const char *c = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));

    Chat *chat = createChat(s, r, c, (uint32_t)strlen(c), false);
    memcpy(chat->id, id, 7);
    chats.push_back(chat);
  }
  sqlite3_finalize(stmt);

  if (rc != SQLITE_DONE) {
    clog_f(TAG, "error: %s", sqlite3_errmsg(db));
    return chats;
  }

  // int rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);
  // if( rc != SQLITE_OK ){
  //     fprintf(stderr, "SQL error: %s\n", zErrMsg);
  //     sqlite3_free(zErrMsg);
  //     return std::vector<echo::Chat *>();
  //  }
  // query db for to=userId
  // for loop {
  //  add result to chats
  //  chat = createChat(, , , ,)
  //  chats.push_back(chat);
  // }
  return chats;
}

void echo::DB::deleteChat(Chat *chat){
  //delete all chats with the id
  char *zErrMsg = 0;
  // std::cout << "deleting chat: " << chat->id << std::endl;
  std::string sql = fmt::format("delete from Chat where id=\"{}\"", chat->id);
  int rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);
  if( rc != SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   }
}

echo::DB *echo::DB::getInstance(){
  if(instance == nullptr){
    instance = new DB();
  }
  return instance;
}

void echo::DB::deleteInstance(){
  if(instance != nullptr){
    //close connection to DB
    delete instance;
  }
}
