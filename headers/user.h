#ifndef USER_H
#define USER_H

#include <string>

class User {
public:
    std::string username;
    std::string phone_number;
    int login_count;

    User() : login_count(0) {}
    User(const std::string& u, const std::string& ph = "", int count = 0)
        : username(u), phone_number(ph), login_count(count) {}
};

#endif