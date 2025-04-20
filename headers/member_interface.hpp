#ifndef MEMBER_INTERFACE_HPP
#define MEMBER_INTERFACE_HPP

#include "interface.h"
#include "cust_table.h"
#include "user_table.h"
#include <iostream>
#include <iomanip>
#include <string>

class member_interface {
private:
    cust_table& customer_table;
    UserTable user_table;
    std::string username;

    void display_header() {
        std::cout << "\n+==================================================+\n";
        std::cout << "|                   GYM MEMBER PORTAL               |\n";
        std::cout << "+==================================================+\n\n";
    }

    void display_member_info() {
        // Find the member in the customer table using phone number
        std::string phone = user_table.get_phone(username);
        bool found = false;
        
        for (const auto& [id, cust] : customer_table.hashtable) {
            if (cust.phone == phone) {
                found = true;
                std::cout << "\n+==================================================+\n";
                std::cout << "|                 MEMBER INFORMATION                 |\n";
                std::cout << "+==================================================+\n\n";
                
                std::cout << "  Name: " << cust.name << "\n";
                std::cout << "  Membership Status: " << cust.status << "\n";
                std::cout << "  Expiry Date: " << cust.expiry_date << "\n";
                std::cout << "  Sessions Purchased: " << cust.sessions_purchased << "\n";
                std::cout << "  Sessions Used: " << cust.sessions_used << "\n";
                std::cout << "  Sessions Remaining: " << (cust.sessions_purchased - cust.sessions_used) << "\n\n";
                
                std::cout << "+==================================================+\n";
                std::cout << "|                  SESSION STATUS                   |\n";
                std::cout << "+==================================================+\n\n";
                
                // Display session usage progress bar
                int total_width = 50;
                int used_width = (cust.sessions_used * total_width) / cust.sessions_purchased;
                int remaining_width = total_width - used_width;
                
                std::cout << "  Progress: [";
                std::cout << std::string(used_width, '#');
                std::cout << std::string(remaining_width, '-');
                std::cout << "] " << cust.sessions_used << "/" << cust.sessions_purchased << " sessions\n\n";
                
                if (cust.sessions_purchased - cust.sessions_used <= 5) {
                    std::cout << "  ! Warning: You have " << (cust.sessions_purchased - cust.sessions_used) 
                              << " sessions remaining. Consider purchasing more sessions.\n\n";
                }
                
                if (cust.status != "Active") {
                    std::cout << "  ! Your membership is currently " << cust.status << ". Please contact the gym to reactivate.\n\n";
                }
                break;
            }
        }
        
        if (!found) {
            std::cout << "\n* No membership information found for your account.\n";
            std::cout << "Please contact the gym administration to link your account.\n\n";
        }
    }

    void add_session() {
        std::string phone = user_table.get_phone(username);
        bool found = false;
        
        for (auto& [id, cust] : customer_table.hashtable) {
            if (cust.phone == phone) {
                found = true;
                if (cust.status != "Active") {
                    std::cout << "\n* Cannot add session. Your membership is currently " << cust.status << ".\n";
                    std::cout << "  Please contact the gym to reactivate your membership.\n\n";
                    return;
                }
                
                if (cust.sessions_used >= cust.sessions_purchased) {
                    std::cout << "\n* Cannot add session. You have used all your purchased sessions.\n";
                    std::cout << "  Please purchase more sessions to continue.\n\n";
                    return;
                }
                
                cust.sessions_used++;
                std::cout << "\n* Session added successfully!\n";
                std::cout << "  Sessions remaining: " << (cust.sessions_purchased - cust.sessions_used) << "\n\n";
                customer_table.write_data();  // Save changes to database
                break;
            }
        }
        
        if (!found) {
            std::cout << "\n* Error: Could not find your membership information.\n";
            std::cout << "  Please contact the gym administration.\n\n";
        }
    }

public:
    member_interface(cust_table& table, const std::string& uname) 
        : customer_table(table), username(uname) {}

    bool show_interface() {
        while (true) {
            display_header();
            display_member_info();
            
            std::cout << "\nOptions:\n";
            std::cout << "1. Refresh Information\n";
            std::cout << "2. Add Session\n";
            std::cout << "3. Logout\n";
            std::cout << "Enter your choice (1-3): ";
            
            int choice;
            std::cin >> choice;
            
            switch (choice) {
                case 1:
                    // Refresh is automatic on next loop
                    break;
                case 2:
                    add_session();
                    break;
                case 3:
                    std::cout << "\nLogging out...\n";
                    return true;  // Return true to indicate logout
                default:
                    std::cout << "\nInvalid choice. Please try again.\n";
            }
        }
    }
};

#endif 