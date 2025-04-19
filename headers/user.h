// user.h
#ifndef USER_H
#define USER_H

#include <string>

class User {
public:
    std::string username;
    std::string password;
    std::string phone_number;

    User() = default;

    User(const std::string& u, const std::string& p, const std::string& ph = "") 
        : username(u), password(p), phone_number(ph) {}
};

#endif
