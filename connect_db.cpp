#include <iostream>
#include "sqlite3.h"
static int callback(void* NotUsed, int argc, char** argv, char** azColName) {
    for (int i = 0; i < argc; i++) {
        std::cout << azColName[i] << " = " << (argv[i] ? argv[i] : "NULL") << "\n";
    }
    return 0;
}

int main() {
    sqlite3* DB;
    char* errorMessage;

    int exit = sqlite3_open("test.db", &DB);
    if (exit) {
        std::cerr << "❌ Error opening DB: " << sqlite3_errmsg(DB) << std::endl;
        return -1;
    } else {
        std::cout << "✅ Database opened successfully!\n";
    }

    const char* sql = "CREATE TABLE IF NOT EXISTS users ("
                      "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                      "name TEXT, "
                      "city TEXT, "
                      "state TEXT, "
                      "last_visit DATE, "
                      "total_sales DECIMAL"
                      ");"
                      "SELECT * FROM users;";

    exit = sqlite3_exec(DB, sql, callback, nullptr, &errorMessage);
    if (exit != SQLITE_OK) {
        std::cerr << "❌ SQL error: " << errorMessage << std::endl;
        sqlite3_free(errorMessage);
    } else {
        std::cout << "✅ SQL executed successfully\n";
    }

    sqlite3_close(DB);
    return 0;
}
