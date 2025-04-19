#include <iostream>
#include "headers/interface.hpp"
#include "headers/user_table.h"


int main() {
    UserTable user_table;
    std::string username, password;
    int choice;

    std::cout << "Welcome to the CRM App\n";
    std::cout << "1. Sign Up\n2. Sign In\nEnter choice: ";
    std::cin >> choice;

    std::cout << "Username: ";
    std::cin >> username;
    std::cout << "Password: ";
    std::cin >> password;

    if (choice == 1) {
        if (user_table.signup(username, password)) {
            std::cout << "Sign Up Successful!\n";
        } else {
            std::cout << "Username already exists. Try again.\n";
            return 0;
        }
    } else if (choice == 2) {
        if (!user_table.login(username, password)) {
            std::cout << "Invalid credentials. Exiting.\n";
            return 0;
        }
        std::cout << "Login successful!\n";
    } else {
        std::cout << "Invalid choice.\n";
        return 0;
    }

    // Launch CRM after login
    cust_table table("cust_data.csv");
    interface main_menu(table);
    main_menu.show_interface();
    return 0;
}
