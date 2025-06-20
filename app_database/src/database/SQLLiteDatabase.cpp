#ifdef ENABLE_SQLITE
#include "app_database/SQLiteDatabase.h"
#include <sqlite3.h>
#include <iostream>

namespace app_database {
bool SQLiteDatabase::connect(const std::string& file) {
    return sqlite3_open(file.c_str(), &db) == SQLITE_OK;
}
}
#endif
