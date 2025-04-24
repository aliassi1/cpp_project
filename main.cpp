#include <iostream>
#include "headers/interface.hpp"
#include "headers/member_interface.hpp"
#include "headers/user_table.h"
#include "headers/sha256.h"
#include <conio.h>  // For masking password input (Windows only)
#include <algorithm>
#include "headers/cust_table.h"
#include "headers/cust_table.hpp"

// Function to get hidden input (password) from user
std::string get_hidden_input() {
    std::string input;
    char ch;
    while ((ch = _getch()) != '\r') {  // until Enter key
        if (ch == '\b') {
            if (!input.empty()) {
                std::cout << "\b \b";
                input.pop_back();
            }
        } else {
            input += ch;
            std::cout << '*';
        }
    }
    std::cout << std::endl;
    return input;
}

int main() {
    try {
        // Initialize the customer table with SQLite database
        cust_table table("test.db");
        UserTable user_table;

        while (true) {
            try {
                std::string username, phone;

                std::cout << "\n+==================================================+\n";
                std::cout << "|                   GYM LOGIN PORTAL              |\n";
                std::cout << "+==================================================+\n\n";

                // Ask if user wants to log in as admin or member
                std::string login_type;
                std::cout << "Enter 'admin' to log in as admin or 'member' to log in as a member: ";
                std::cin >> login_type;

                if (login_type == "admin") {
                    std::string admin_password = "adminpass";
                    std::string password;
                    std::cout << "Enter admin password: ";
                    password = get_hidden_input();

                    if (password == admin_password) {
                        std::cout << "Admin login successful!\n";
                        interface admin_menu(table);
                        admin_menu.show_interface();
                    } else {
                        std::cout << "❌ Invalid admin password.\n";
                    }
                } else if (login_type == "member") {
                    std::cout << "Enter phone number: ";
                    std::cin >> phone;
                    std::cout << "Enter password: ";
                    std::string password = get_hidden_input();

                    if (user_table.login(phone, password)) {
                        std::cout << "Member login successful!\n";
                        member_interface member_menu(table, phone);
                        member_menu.show_interface();
                    } else {
                        std::cout << "❌ Invalid member credentials.\n";
                    }
                } else {
                    std::cout << "Invalid login type. Please enter 'admin' or 'member'.\n";
                }
            } catch (const std::exception& e) {
                std::cerr << "Error during login: " << e.what() << std::endl;
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Critical error: " << e.what() << std::endl;
        return 1;
    }
}