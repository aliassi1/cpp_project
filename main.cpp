/* main.cpp
Final project
Nuts n' Bolts Simple CRM
Name: Jarred Glaser
Class: SDEV-240
Description: This is a simple CRM console application. See Documentation For more details.*/

#include <iostream>
#include "headers/interface.hpp"

int main() {
    // Initialize the customer table with SQLite database
    cust_table table("test.db");
    // Create an interface object
    interface main_menu(table);
    // Run the main interface process
    main_menu.show_interface();
    return 0;
}


