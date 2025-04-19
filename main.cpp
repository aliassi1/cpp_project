#include <iostream>
#include "headers/interface.hpp"
#include "headers/user_table.h"
#include "headers/sha256.h"
#include <conio.h>  // For masking password input (Windows only)

const std::string ADMIN_USERNAME = "admin";
const std::string ADMIN_HASH = "713bfda78870bf9d1b261f565286f85e97ee614efe5f0faf7c34e7ca4f65baca"; // SHA-256 for "adminpass"

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

int main() {
    while (true) {
        UserTable user_table;
        std::string username, password;
        int choice;

        std::cout << "\nWelcome to the Fitness Center CRM\n";
        std::cout << "1. Sign Up\n2. Sign In\nEnter choice: ";
        std::cin >> choice;

        std::cout << "Username: ";
        std::cin >> username;
        std::cout << "Password: ";
        password = get_hidden_input();

        std::string hashed = sha256(password);

        if (choice == 1) {
            if (username == ADMIN_USERNAME) {
                std::cout << "Cannot sign up as admin. Try another username.\n";
                continue;
            }

            if (user_table.signup(username, password)) {
                std::cout << "Sign Up Successful!\n";
            } else {
                std::cout << "Username already exists. Try again.\n";
            }
            continue;
        } 
        else if (choice == 2) {
            if (username == ADMIN_USERNAME && hashed == ADMIN_HASH) {
                std::cout << "Admin login successful!\n";
                cust_table table("cust_data.csv");
                interface admin_menu(table);
                admin_menu.show_interface();
            }
            else if (user_table.login(username, password)) {
                std::cout << "Member login successful!\n";
                cust_table table("cust_data.csv");
                interface member_menu(table);
                member_menu.show_member_view(username);
            } 
            else {
                std::cout << "Invalid credentials.\n";
                continue;
            }
        } 
        else {
            std::cout << "Invalid choice.\n";
            continue;
        }

        std::string again;
        std::cout << "\nLogout? (y/n): ";
        std::cin >> again;
        if (again != "y" && again != "Y") break;
    }

    std::cout << "Goodbye!\n";
    return 0;
}
