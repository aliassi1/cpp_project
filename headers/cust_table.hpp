/*
cust_table.hpp
Member function definitions for cust_table class
*/

#include <fstream>
#include <sstream>
#include <iomanip>
#include <limits>
#include "cust_table.h"
#include "../sqlite3.h"

// Function to get max id in customer table
//int cust_table::get_max_id() {
    //return hashtable.end()->first;
//}
int cust_table::get_max_id() {
    int max_id = 0;
    for (auto it = hashtable.begin(); it != hashtable.end(); ++it) {
        if (it->second.id > max_id) {
            max_id = it->second.id;
        }
    }
    return max_id;
}

// Print the customer table in a neatly formatted way
void cust_table::print_table(int n_rows) {
    // Main Header
    cout << std::left << "|" << std::setw(85) << std::setfill('-') << "-" << "|" << endl
    << "|" << std::setw(85) << std::setfill(' ') << " " << "|" << endl
    << "|" << std::setw(85) << "Nuts n' Bolts Customer Management System" << "|" << endl
    << "|" << std::setw(85) << std::setfill(' ') << " " << "|" << endl;

    // Start headers line
    cout << "|" << std::setw(5) << std::setfill('=') << "="
    << "|" << std::setw(20) << "="
    << "|" << std::setw(15) << "="
    << "|" << std::setw(10) << "="
    << "|" << std::setw(15) << "="
    << "|" << std::setw(15) << "="
    << "|" << std::setfill(' ') << endl;

    // Column headers
    std::cout << std::left
    << "|" << std::setw(5) << "ID"
    << "|" << std::setw(20) << "Name"
    << "|" << std::setw(15) << "City"
    << "|" << std::setw(10) << "State"
    << "|" << std::setw(15) << "Last Visit"
    << "|" << std::setw(15) << "Total Sales"
    << "|" << endl
    << "|" << std::setw(5) << std::setfill('=') << "="
    << "|" << std::setw(20) << "="
    << "|" << std::setw(15) << "="
    << "|" << std::setw(10) << "="
    << "|" << std::setw(15) << "="
    << "|" << std::setw(15) << "="
    << "|" << std::setfill(' ') << endl;
    int iter = 0;
    // Print table rows
    for (auto it = hashtable.begin(); it != hashtable.end(); ++it, ++iter) {
            if (iter == n_rows) {
                cout << "Showing " + std::to_string(iter) + " of " + std::to_string(hashtable.size()) + " customers." << endl;
                break;
            }
            cout << std::left 
            << "|" << std::setw(5) << it -> first 
            << "|" << std::setw(20) << it->second.name 
            << "|" << std::setw(15) << it->second.city
            << "|" << std::setw(10) << it->second.state
            << "|" << std::setw(15) << it->second.format_date()
            << "|" << std::setw(15) << "$" + std::to_string(it->second.total_sales)
            << "|" << endl
            << "|" << std::setw(5) << std::setfill('-') << "-"
            << "|" << std::setw(20) << "-"
            << "|" << std::setw(15) << "-"
            << "|" << std::setw(10) << "-"
            << "|" << std::setw(15) << "-"
            << "|" << std::setw(15) << "-"
            << "|"
            << std::setfill(' ')
            << endl;
        }
    if (iter >= hashtable.size()) {
        cout << "Showing all customers" << endl;
    }
}

// Initialize the SQLite database
void cust_table::init_database() {
    int rc = sqlite3_open(db_name.c_str(), &db);
    if (rc) {
        cerr << "Can't open database: " << sqlite3_errmsg(db) << endl;
        return;
    }
    
    // Create table if it doesn't exist
    const char* sql = "CREATE TABLE IF NOT EXISTS customers ("
                     "id INTEGER PRIMARY KEY,"
                     "name TEXT,"
                     "city TEXT,"
                     "state TEXT,"
                     "last_visit INTEGER,"
                     "total_sales INTEGER);";
                     
    char* errMsg = 0;
    rc = sqlite3_exec(db, sql, 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        cerr << "SQL error: " << errMsg << endl;
        sqlite3_free(errMsg);
    }
}

// Write customer table to database
void cust_table::write_data() {
    // Begin transaction for better performance
    sqlite3_exec(db, "BEGIN TRANSACTION", 0, 0, 0);
    
    // Prepare the insert statement
    const char* sql = "INSERT OR REPLACE INTO customers (id, name, city, state, last_visit, total_sales) "
                     "VALUES (?, ?, ?, ?, ?, ?);";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    
    for (auto it = hashtable.begin(); it != hashtable.end(); ++it) {
        sqlite3_bind_int(stmt, 1, it->first);
        sqlite3_bind_text(stmt, 2, it->second.name.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 3, it->second.city.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 4, it->second.state.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 5, it->second.last_visit);
        sqlite3_bind_int(stmt, 6, it->second.total_sales);
        
        sqlite3_step(stmt);
        sqlite3_reset(stmt);
    }
    
    sqlite3_finalize(stmt);
    sqlite3_exec(db, "COMMIT", 0, 0, 0);
}

// Read customer data from database
void cust_table::read_data() {
    const char* sql = "SELECT * FROM customers;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        string name = (const char*)sqlite3_column_text(stmt, 1);
        string city = (const char*)sqlite3_column_text(stmt, 2);
        string state = (const char*)sqlite3_column_text(stmt, 3);
        int last_visit = sqlite3_column_int(stmt, 4);
        int total_sales = sqlite3_column_int(stmt, 5);
        
        customer cust(id, name, last_visit, total_sales, city, state);
        insert_row(id, cust);
    }
    
    sqlite3_finalize(stmt);
}

// Get total sales of all customers
int cust_table::get_total_sales() {
    int total_sales_all = 0;
    for (auto it = hashtable.begin(); it != hashtable.end(); ++it) {
        total_sales_all += it->second.total_sales;
    }
    return total_sales_all;
}