#include <iostream>
#include "headers/interface.hpp"
#include "headers/user_table.h"
#include "headers/sha256.h"
#include <conio.h>  // For masking password input (Windows only)
#include <regex>
#include <stdexcept>

const std::string ADMIN_USERNAME = "admin";
const std::string ADMIN_HASH = "713bfda78870bf9d1b261f565286f85e97ee614efe5f0faf7c34e7ca4f65baca"; // SHA-256 for "adminpass"

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

bool is_valid_password(const std::string& password) {
    if (password.empty()) {
        std::cout << "Password cannot be empty.\n";
        return false;
    }
    if (password.length() < 6) {
        std::cout << "Password must be at least 6 characters long.\n";
        return false;
    }
    return true;
}

std::string get_hidden_input() {
    std::string password;
    char ch;
    while ((ch = _getch()) != '\r') {  // until Enter key
        if (ch == '\b') {
            if (!password.empty()) {
                std::cout << "\b \b";
                password.pop_back();
            }
        } else {
            password += ch;
            std::cout << '*';
        }
    }
    std::cout << std::endl;
    return password;
}

int get_valid_choice() {
    int choice;
    while (true) {
        std::cout << "1. Sign Up\n2. Sign In\nEnter choice: ";
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Please enter a valid number.\n";
            continue;
        }
        if (choice != 1 && choice != 2) {
            std::cout << "Please enter either 1 or 2.\n";
            continue;
        }
        return choice;
    }
}

int main() {
    try {
        // Initialize the customer table with SQLite database
        cust_table table("test.db");
        
        while (true) {
            try {
                UserTable user_table;
                std::string username, password;
                int choice = get_valid_choice();

                // Get and validate username
                do {
                    std::cout << "Username: ";
                    std::cin >> username;
                } while (!is_valid_username(username));

                // Get and validate password
                do {
                    std::cout << "Password: ";
                    password = get_hidden_input();
                } while (!is_valid_password(password));

                std::string hashed = sha256(password);

                if (choice == 1) {
                    if (username == ADMIN_USERNAME) {
                        std::cout << "Cannot sign up as admin. Try another username.\n";
                        continue;
                    }

                    try {
                        if (user_table.signup(username, password)) {
                            std::cout << "Sign Up Successful!\n";
                        } else {
                            std::cout << "Username already exists. Try again.\n";
                        }
                    } catch (const std::exception& e) {
                        std::cerr << "Error during signup: " << e.what() << std::endl;
                    }
                    continue;
                } 
                else if (choice == 2) {
                    try {
                        if (username == ADMIN_USERNAME && hashed == ADMIN_HASH) {
                            std::cout << "Admin login successful!\n";
                            interface admin_menu(table);
                            admin_menu.show_interface();
                        }
                        else if (user_table.login(username, password)) {
                            std::cout << "Member login successful!\n";
                            interface member_menu(table);
                            member_menu.show_member_view(username);
                        } 
                        else {
                            std::cout << "Invalid credentials.\n";
                            continue;
                        }
                    } catch (const std::exception& e) {
                        std::cerr << "Error during login: " << e.what() << std::endl;
                        continue;
                    }
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
