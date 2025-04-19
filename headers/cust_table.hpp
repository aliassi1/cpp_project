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
    std::cout << "Database opened successfully: " << db_name << std::endl;

    const char* sql = "CREATE TABLE IF NOT EXISTS users ("
                      "id INTEGER PRIMARY KEY, "
                      "name TEXT, "
                      "phone TEXT, "
                      "city TEXT, "
                      "expiry_date DATE, "
                      "sessions_purchased INTEGER, "
                      "sessions_used INTEGER, "
                      "status TEXT, "
                      "total_paid DECIMAL);";

    char* errMsg = nullptr;
    rc = sqlite3_exec(db, sql, nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    } else {
        std::cout << "Table 'users' created or already exists" << std::endl;
    }
}

// Write customer table to database
void cust_table::write_data() {
    std::cout << "Starting database write operation..." << std::endl;
    
    int rc = sqlite3_exec(db, "BEGIN TRANSACTION", nullptr, nullptr, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to begin transaction: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    const char* sql = "INSERT OR REPLACE INTO users "
                      "(id, name, phone, city, expiry_date, sessions_purchased, sessions_used, status, total_paid) "
                      "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?);";

    sqlite3_stmt* stmt;
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_exec(db, "ROLLBACK", nullptr, nullptr, nullptr);
        return;
    }

    int rows_affected = 0;
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

        rc = sqlite3_step(stmt);
        if (rc != SQLITE_DONE) {
            std::cerr << "Failed to insert row " << id << ": " << sqlite3_errmsg(db) << std::endl;
        } else {
            rows_affected++;
        }
        sqlite3_reset(stmt);
    }

    sqlite3_finalize(stmt);
    
    rc = sqlite3_exec(db, "COMMIT", nullptr, nullptr, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to commit transaction: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_exec(db, "ROLLBACK", nullptr, nullptr, nullptr);
        return;
    }

    std::cout << "Database write completed. Rows affected: " << rows_affected << std::endl;
}

// Read customer data from database
void cust_table::read_data() {
    std::cout << "Starting database read operation..." << std::endl;
    
    const char* sql = "SELECT * FROM users;";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    int rows_read = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        
        // Add null checks for text columns
        const char* name_text = (const char*)sqlite3_column_text(stmt, 1);
        const char* phone_text = (const char*)sqlite3_column_text(stmt, 2);
        const char* city_text = (const char*)sqlite3_column_text(stmt, 3);
        const char* expiry_text = (const char*)sqlite3_column_text(stmt, 4);
        const char* status_text = (const char*)sqlite3_column_text(stmt, 7);

        std::string name = name_text ? name_text : "";
        std::string phone = phone_text ? phone_text : "";
        std::string city = city_text ? city_text : "";
        std::string expiry_date = expiry_text ? expiry_text : "";
        std::string status = status_text ? status_text : "";

        int sessions_purchased = sqlite3_column_int(stmt, 5);
        int sessions_used = sqlite3_column_int(stmt, 6);
        float total_paid = static_cast<float>(sqlite3_column_double(stmt, 8));

        customer cust(id, name, phone, city, expiry_date, sessions_purchased, status, total_paid);
        cust.sessions_used = sessions_used;
        insert_row(id, cust);
        rows_read++;
    }

    sqlite3_finalize(stmt);
    std::cout << "Database read completed. Rows read: " << rows_read << std::endl;
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

// Print the customer table
void cust_table::print_table(int n_rows) {
    // Print header
    std::cout << std::left
              << std::setw(5) << "ID"
              << std::setw(20) << "Name"
              << std::setw(15) << "Phone"
              << std::setw(15) << "City"
              << std::setw(12) << "Expiry Date"
              << std::setw(10) << "Sessions"
              << std::setw(10) << "Used"
              << std::setw(10) << "Status"
              << std::setw(10) << "Total Paid"
              << std::endl;
    
    std::cout << std::string(107, '-') << std::endl;

    // Print rows
    int count = 0;
    for (const auto& [id, cust] : hashtable) {
        if (n_rows > 0 && count >= n_rows) break;
        
        std::cout << std::left
                  << std::setw(5) << cust.id
                  << std::setw(20) << cust.name
                  << std::setw(15) << cust.phone
                  << std::setw(15) << cust.city
                  << std::setw(12) << cust.expiry_date
                  << std::setw(10) << cust.sessions_purchased
                  << std::setw(10) << cust.sessions_used
                  << std::setw(10) << cust.status
                  << std::fixed << std::setprecision(2) << cust.total_paid
                  << std::endl;
        
        count++;
    }
}
