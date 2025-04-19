#include <sstream>
#include <iomanip>
#include <limits>
#include "cust_table.h"
#include "../sqlite3.h"

// Initialize the SQLite database
void cust_table::init_database() {
    int rc = sqlite3_open(db_name.c_str(), &db);
    if (rc) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    const char* sql = "CREATE TABLE IF NOT EXISTS customers ("
                      "id INTEGER PRIMARY KEY, "
                      "name TEXT, "
                      "phone TEXT, "
                      "city TEXT, "
                      "expiry_date TEXT, "
                      "sessions_purchased INTEGER, "
                      "sessions_used INTEGER, "
                      "status TEXT, "
                      "total_paid REAL);";

    char* errMsg = nullptr;
    rc = sqlite3_exec(db, sql, nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
}

// Write customer table to database
void cust_table::write_data() {
    sqlite3_exec(db, "BEGIN TRANSACTION", nullptr, nullptr, nullptr);

    const char* sql = "INSERT OR REPLACE INTO customers "
                      "(id, name, phone, city, expiry_date, sessions_purchased, sessions_used, status, total_paid) "
                      "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?);";

    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

    for (const auto& [id, cust] : hashtable) {
        sqlite3_bind_int(stmt, 1, cust.id);
        sqlite3_bind_text(stmt, 2, cust.name.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 3, cust.phone.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 4, cust.city.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 5, cust.expiry_date.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 6, cust.sessions_purchased);
        sqlite3_bind_int(stmt, 7, cust.sessions_used);
        sqlite3_bind_text(stmt, 8, cust.status.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_double(stmt, 9, cust.total_paid);

        sqlite3_step(stmt);
        sqlite3_reset(stmt);
    }

    sqlite3_finalize(stmt);
    sqlite3_exec(db, "COMMIT", nullptr, nullptr, nullptr);
}

// Read customer data from database
void cust_table::read_data() {
    const char* sql = "SELECT * FROM customers;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        std::string name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        std::string phone = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        std::string city = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        std::string expiry_date = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
        int sessions_purchased = sqlite3_column_int(stmt, 5);
        int sessions_used = sqlite3_column_int(stmt, 6);
        std::string status = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 7));
        float total_paid = static_cast<float>(sqlite3_column_double(stmt, 8));

        customer cust(id, name, phone, city, expiry_date, sessions_purchased, status, total_paid);
        cust.sessions_used = sessions_used;
        insert_row(id, cust);
    }

    sqlite3_finalize(stmt);
}

// Get the max customer ID
int cust_table::get_max_id() {
    int max_id = 0;
    for (const auto& [id, cust] : hashtable) {
        if (cust.id > max_id) {
            max_id = cust.id;
        }
    }
    return max_id;
}

// Sum all total_paid values
float cust_table::get_total_paid() {
    float total = 0.0f;
    for (const auto& [_, cust] : hashtable) {
        total += cust.total_paid;
    }
    return total;
}
