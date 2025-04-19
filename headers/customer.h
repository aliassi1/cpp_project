#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <string>

class customer {
public:
    int id;
    std::string name;
    std::string phone;
    std::string city;
    std::string expiry_date;
    int sessions_purchased;
    int sessions_used;
    std::string status;
    float total_paid;

    customer() = default;

    customer(int i_id, std::string i_name, std::string i_phone, std::string i_city,
             std::string i_expiry, int i_sessions_purchased, std::string i_status, float i_total_paid)
        : id(i_id), name(i_name), phone(i_phone), city(i_city),
          expiry_date(i_expiry), sessions_purchased(i_sessions_purchased),
          sessions_used(0), status(i_status), total_paid(i_total_paid) {}

    std::string format_date() const {
        return expiry_date;
    }
};

#endif
