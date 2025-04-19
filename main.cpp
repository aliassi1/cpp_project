#include <iostream>
#include "headers/interface.hpp"
#include "headers/member_interface.hpp"
#include "headers/user_table.h"
#include "headers/sha256.h"
#include <conio.h>  // For masking password input (Windows only)
#include <regex>
#include <stdexcept>

const std::string ADMIN_USERNAME = "admin";
const std::string ADMIN_PASSWORD = "adminpass";

// Input validation functions
bool is_valid_username(const std::string& username) {
    if (username.empty()) {
        std::cout << "Username cannot be empty.\n";
        return false;
    }
    // Allow only alphanumeric characters and underscore
    std::regex username_pattern("^[a-zA-Z0-9_]+$");
    if (!std::regex_match(username, username_pattern)) {
        std::cout << "Username can only contain letters, numbers, and underscores.\n";
        return false;
    }
    return true;
}

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

// Create a username from a name
std::string create_username(const std::string& name) {
    std::string username = name;
    // Convert to lowercase
    std::transform(username.begin(), username.end(), username.begin(), ::tolower);
    // Replace spaces with underscores
    std::replace(username.begin(), username.end(), ' ', '_');
    // Remove any non-alphanumeric characters
    username.erase(std::remove_if(username.begin(), username.end(), 
                  [](char c) { return !std::isalnum(c) && c != '_'; }), username.end());
    return username;
}

int main() {
    try {
        // Initialize the customer table with SQLite database
        cust_table table("test.db");
        
        // Load customer data from database
        std::cout << "Loading customer data...\n";
        table.read_data();
        std::cout << "Customer data loaded successfully.\n\n";
        
        // Create user accounts for existing customers
        UserTable user_table;
        int accounts_created = 0;
        
        std::cout << "Creating user accounts for existing customers...\n";
        for (const auto& [id, cust] : table.hashtable) {
            std::string username = create_username(cust.name);
            if (user_table.add_user(username, cust.phone)) {
                std::cout << "Created account for " << cust.name << " with username: " << username << "\n";
                accounts_created++;
            }
        }
        std::cout << "Created " << accounts_created << " user accounts.\n\n";
        
        while (true) {
            try {
                std::string username, phone;
                
                std::cout << "\n+==================================================+\n";
                std::cout << "|                   GYM LOGIN PORTAL                 |\n";
                std::cout << "+==================================================+\n\n";

                // Get and validate username
                do {
                    std::cout << "Username: ";
                    std::cin >> username;
                } while (!is_valid_username(username));

                // Get phone number/password
                std::cout << "Phone Number/Password: ";
                phone = get_hidden_input();

                try {
                    if (username == ADMIN_USERNAME && phone == ADMIN_PASSWORD) {
                        std::cout << "Admin login successful!\n";
                        interface admin_menu(table);
                        admin_menu.show_interface();
                    }
                    else {
                        // Debug output
                        std::cout << "Attempting member login...\n";
                        std::cout << "Username: " << username << "\n";
                        std::cout << "Phone: " << phone << "\n";
                        
                        if (user_table.login(username, phone)) {
                            std::cout << "Member login successful!\n";
                            member_interface member_menu(table, username);
                            member_menu.show_interface();
                        } 
                        else {
                            std::cout << "Invalid credentials.\n";
                            std::cout << "Please make sure you're using your registered username and phone number.\n";
                            continue;
                        }
                    }
                } catch (const std::exception& e) {
                    std::cerr << "Error during login: " << e.what() << std::endl;
                    continue;
                }

                std::string again;
                do {
                    std::cout << "\nLogout? (y/n): ";
                    std::cin >> again;
                } while (again != "y" && again != "Y" && again != "n" && again != "N");
                
                if (again == "y" || again == "Y") break;
            } catch (const std::exception& e) {
                std::cerr << "Error in main loop: " << e.what() << std::endl;
                std::cout << "Please try again.\n";
                continue;
            }
        }

        std::cout << "Goodbye!\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Critical error: " << e.what() << std::endl;
        return 1;
    }
}
