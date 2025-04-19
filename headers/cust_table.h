// cust_table.h
// Customer table class for CRM — now backed by SQLite

#ifndef __cust_table_H_INCLUDED__ 
#define __cust_table_H_INCLUDED__ 

#include <iostream>
#include "../sqlite3.h"
#include "table.h"
#include "customer.hpp"

class cust_table : public table<int, customer> {
private:
    sqlite3* db;
    void init_database();  // Ensures table exists

public:
    std::string db_name;

    // Constructor - assign params and fill table with data on class creation
    cust_table(std::string i_db_name)
        : db_name(i_db_name)
    {
        init_database();
        read_data();
    }

    // Destructor - save the data to database and cleanup
    ~cust_table() {
        write_data();
        hashtable.clear();
        sqlite3_close(db);
    }

    // Member functions
    void print_table(int n_rows);
    void read_data();
    void write_data();
    int get_max_id();
    float get_total_paid();  // ✅ Your updated method
};

#include "cust_table.hpp"

#endif
