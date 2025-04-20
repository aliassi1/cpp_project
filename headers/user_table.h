// user_table.h
#ifndef USER_TABLE_H
#define USER_TABLE_H

#include "user.h"
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include "sha256.h"

class UserTable {
private:
    std::vector<User> users;
    std::string filename = "user_data.csv";

public:
    UserTable() {
        load_users();
    }

    void load_users() {
        users.clear();
        std::ifstream file(filename);
        if (!file.is_open()) {
            return;
        }
        
        std::string line, username, password, phone;
        while (std::getline(file, line)) {
            std::stringstream ss(line);
            std::getline(ss, username, ',');
            std::getline(ss, password, ',');
            std::getline(ss, phone, ',');
            users.emplace_back(username, password, phone);
        }
        file.close();
    }

    void save_users() {
        std::ofstream file(filename);
        for (const auto& user : users) {
            file << user.username << "," << user.password << "," << user.phone_number << "\n";
        }
        file.close();
    }

    bool add_user(const std::string& username, const std::string& phone) {
        if (username == "admin") return false; // prevent admin registration

        for (const auto& user : users) {
            if (user.username == username) return false; // already exists
        }

        users.emplace_back(username, "", phone); // Empty password since we're using phone for login
        save_users();
        return true;
    }

    bool login(const std::string& username, const std::string& phone) {
        for (const auto& user : users) {
            if (user.username == username && user.phone_number == phone) {
                std::cout << "Login match found!\n";
                return true;
            }
        }
        std::cout << "No matching user found.\n";
        return false;
    }

    std::string get_phone(const std::string& username) {
        for (const auto& user : users) {
            if (user.username == username) {
                return user.phone_number;
            }
        }
        return "";
    }
};

#endif
