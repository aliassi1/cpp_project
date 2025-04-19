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
        std::string line, username, password;
        while (std::getline(file, line)) {
            std::stringstream ss(line);
            std::getline(ss, username, ',');
            std::getline(ss, password, ',');
            users.emplace_back(username, password);
        }
    }

    void save_users() {
        std::ofstream file(filename);
        for (const auto& user : users) {
            file << user.username << "," << user.password << "\n";
        }
    }

    bool signup(const std::string& username, const std::string& password) {
        if (username == "admin") return false; // prevent admin registration

        for (const auto& user : users) {
            if (user.username == username) return false; // already exists
        }

        std::string hashed_password = sha256(password);
        users.emplace_back(username, hashed_password);
        save_users();
        return true;
    }

    bool login(const std::string& username, const std::string& password) {
        std::string hashed_password = sha256(password);
        for (const auto& user : users) {
            if (user.username == username && user.password == hashed_password)
                return true;
        }
        return false;
    }
};

#endif
