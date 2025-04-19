/*
cust_table.hpp
Member function definitions for cust_table class
*/

#include <fstream>
#include <sstream>
#include <iomanip>
#include <limits>
#include "cust_table.h"

// Get max id in customer table
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
    std::cout << std::left << "|" << std::setw(115) << std::setfill('-') << "-" << "|" << std::endl
              << "|" << std::setw(115) << std::setfill(' ') << " " << "|" << std::endl
              << "|" << std::setw(115) << "Nuts n' Bolts Customer Management System" << "|" << std::endl
              << "|" << std::setw(115) << std::setfill(' ') << " " << "|" << std::endl;

    // Start header lines
    std::cout << "|" << std::setw(5) << std::setfill('=') << "="
              << "|" << std::setw(20) << "="
              << "|" << std::setw(15) << "="
              << "|" << std::setw(15) << "="
              << "|" << std::setw(15) << "="
              << "|" << std::setw(10) << "="
              << "|" << std::setw(15) << "="
              << "|" << std::setw(15) << "="
              << "|" << std::setfill(' ') << std::endl;

    // Column headers
    std::cout << std::left
              << "|" << std::setw(5)  << "ID"
              << "|" << std::setw(20) << "Name"
              << "|" << std::setw(15) << "Phone"
              << "|" << std::setw(15) << "City"
              << "|" << std::setw(15) << "Expiry Date"
              << "|" << std::setw(10) << "Sessions"
              << "|" << std::setw(15) << "Status"
              << "|" << std::setw(15) << "Total Paid"
              << "|" << std::endl;

    std::cout << "|" << std::setw(5) << std::setfill('=') << "="
              << "|" << std::setw(20) << "="
              << "|" << std::setw(15) << "="
              << "|" << std::setw(15) << "="
              << "|" << std::setw(15) << "="
              << "|" << std::setw(10) << "="
              << "|" << std::setw(15) << "="
              << "|" << std::setw(15) << "="
              << "|" << std::setfill(' ') << std::endl;

    int iter = 0;

    // Print rows
    for (const auto& [id, cust] : hashtable) {
        if (iter == n_rows) {
            std::cout << "Showing " << iter << " of " << hashtable.size() << " customers.\n";
            break;
        }

        std::cout << "|" << std::setw(5)  << cust.id
                  << "|" << std::setw(20) << cust.name
                  << "|" << std::setw(15) << cust.phone
                  << "|" << std::setw(15) << cust.city
                  << "|" << std::setw(15) << cust.expiry_date
                  << "|" << std::setw(10) << cust.sessions_used
                  << "|" << std::setw(15) << cust.status
                  << "|" << std::setw(15) << ("$" + std::to_string(cust.total_paid))
                  << "|" << std::endl;

        std::cout << "|" << std::setw(5) << std::setfill('-') << "-"
                  << "|" << std::setw(20) << "-"
                  << "|" << std::setw(15) << "-"
                  << "|" << std::setw(15) << "-"
                  << "|" << std::setw(15) << "-"
                  << "|" << std::setw(10) << "-"
                  << "|" << std::setw(15) << "-"
                  << "|" << std::setw(15) << "-"
                  << "|" << std::setfill(' ') << std::endl;

        ++iter;
    }

    if (iter >= hashtable.size()) {
        std::cout << "Showing all customers" << std::endl;
    }
}

// Write customer table to local db
void cust_table::write_data() {
    std::ofstream file(filename);
    for (const auto& [id, cust] : hashtable) {
        file << cust.id << ","
             << cust.name << ","
             << cust.phone << ","
             << cust.city << ","
             << cust.expiry_date << ","
             << cust.sessions_purchased << ","
             << cust.sessions_used << ","
             << cust.status << ","
             << cust.total_paid << "\n";
    }
}

void cust_table::read_data() {
    std::ifstream file(filename);
    if (!file) return;

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string id_str, name, phone, city, expiry_date;
        std::string sessions_purchased_str, sessions_used_str, status, total_paid_str;

        std::getline(ss, id_str, ',');
        std::getline(ss, name, ',');
        std::getline(ss, phone, ',');
        std::getline(ss, city, ',');
        std::getline(ss, expiry_date, ',');
        std::getline(ss, sessions_purchased_str, ',');
        std::getline(ss, sessions_used_str, ',');
        std::getline(ss, status, ',');
        std::getline(ss, total_paid_str, ',');

        int id = std::stoi(id_str);
        int sessions_purchased = std::stoi(sessions_purchased_str);
        int sessions_used = std::stoi(sessions_used_str);
        float total_paid = std::stof(total_paid_str);

        customer c(id, name, phone, city, expiry_date, sessions_purchased, status, total_paid);
        c.sessions_used = sessions_used; // set separately
        hashtable[id] = c;
    }
}

float cust_table::get_total_paid() {
    float total = 0.0f;
    for (const auto& [_, cust] : hashtable) {
        total += cust.total_paid;
    }
    return total;
}
