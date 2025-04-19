#include <iostream>
#include "headers/interface.hpp"
#include "headers/user_table.h"
#include <conio.h>  // For masking password input (Windows only)

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

        std::cout << "\nWelcome to the CRM App\n";
        std::cout << "1. Sign Up\n2. Sign In\nEnter choice: ";
        std::cin >> choice;

        std::cout << "Username: ";
        std::cin >> username;
        std::cout << "Password: ";
        password = get_hidden_input();  // ✅ Mask input

        if (choice == 1) {
            if (user_table.signup(username, password)) {
                std::cout << "Sign Up Successful!\n";
            } else {
                std::cout << "Username already exists. Try again.\n";
                continue;
            }
        } else if (choice == 2) {
            if (!user_table.login(username, password)) {
                std::cout << "Invalid credentials. Exiting.\n";
                continue;
            }
            std::cout << "Login successful!\n";
        } else {
            std::cout << "Invalid choice.\n";
            continue;
        }

        // Run CRM interface
        cust_table table("cust_data.csv");
        interface main_menu(table);
        main_menu.show_interface();

        // Ask if user wants to logout
        std::string again;
        std::cout << "\nLogout? (y/n): ";
        std::cin >> again;
        if (again != "y" && again != "Y") break;
    }

    std::cout << "Goodbye!\n";
    return 0;
}
