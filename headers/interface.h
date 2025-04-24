#ifndef INTERFACE_H
#define INTERFACE_H

#include <iostream>
#include "cust_table.h"

class interface {
public:
    // Reference to customer table - all input actions will affect this table
    cust_table& customer_table;

    // Constructor - Initializes the interface with a reference to the customer table
    interface(cust_table& i_cust_table)
        : customer_table(i_cust_table)
    {}

    // Displays the main interface for managing customers
    void show_interface();

    // Shows available options for the user (admin)
    void show_options();

    // Handles adding a new customer to the table
    void handle_add_cust();

    // Handles updating an existing customer's information
    void handle_update_cust();

    // Handles deleting a customer from the table
    void handle_delete_cust();

    // Searches for a customer based on user input
    void search_customer();

    // Displays the member-specific view (e.g., profile or session details)
    void show_member_view(const std::string& username);  

    // Handles member session usage (e.g., reduce sessions_used)
    void use_session();
};

#endif
