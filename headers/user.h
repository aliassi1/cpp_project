// user.h
#ifndef USER_H
#define USER_H

#include <string>

class User {
public:
    std::string username;
    std::string password;

    User() = default;
    User(std::string u, std::string p) : username(u), password(p) {}
};

#endif
