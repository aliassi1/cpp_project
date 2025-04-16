/* interface.hpp
interface member function definitions */
#include "interface.h"

// This function handles adding a new customer to the customer table
void interface::handle_add_cust() {
    std::string name, city, state;
    int last_visit, total_sales;
    int last_id = customer_table.get_max_id();
    cin.ignore (std::numeric_limits<std::streamsize>::max(), '\n'); 
    cout << "Enter Customer first and last name: ";
    std::getline(cin,name);
    cout << "Enter Customer City: ";
    std::getline(cin,city,'\n');
    cout << "Enter Customer State: ";
    std::getline(cin,state,'\n');
    cout << "Enter Customer's Last Visit (YYYYMMDD): ";
    cin >> last_visit;
    cout << "Enter Customer's Total Sales: ";
    cin >> total_sales;
    customer new_cust(last_id+1,name,last_visit,total_sales,city,state);
    // The id of the new customer should be the max existing ID incremented by 1
    customer_table.insert_row(last_id+1,new_cust);
}

// Function that handles deleting a customer from the table
void interface::handle_delete_cust() {
    int delete_id;
    cout << "Enter customer ID to delete: ";
    cin >> delete_id;
    // Search table for the id
    auto search = customer_table.hashtable.find(delete_id);
    // If the id is found then carry out process else warn user
    if (search != customer_table.hashtable.end()) {
        cout << "Found customer ID " << search->first << " with the following information: " << endl;
        cout << "Name: " << search->second.name << endl;
        cout << "City: " << search->second.city << endl;
        cout << "State: " << search->second.state << endl;
        cout << "Last Visit: " << search->second.format_date() << endl;
        cout << "Total Sales: $" << search->second.total_sales << endl;
        cout << "Are you sure you want to delete this customer? (Y or N)";
        string ans;
        cin >> ans;
        if (ans == "Y") {
            customer_table.hashtable.erase(search->first);
            cout << "Customer deleted." << endl;
        } else {
            cout << "Customer not deleted." << endl;
        }
    } else {
        cout << "No customer ID " +  std::to_string(delete_id) + " exists." << endl;
    }
}

// Function to handle updating customer
void interface::handle_update_cust() {
    int update_id;
    cout << "Enter customer ID to update: ";
    cin >> update_id;
    
    // Search for customer to update - if found carry out action, else warn user
    auto search = customer_table.hashtable.find(update_id);
    if (search != customer_table.hashtable.end()) {
        std::string name, city, state;
        int last_visit, total_sales;

        cout << "Found customer ID " << search->first << endl;

        // Show current values
        cout << "Current name: " << search->second.name << endl;
        cout << "Current city: " << search->second.city << endl;
        cout << "Current state: " << search->second.state << endl;
        cout << "Current last visit: " << search->second.last_visit << endl;
        cout << "Current total sales: " << search->second.total_sales << endl;

        // Ask if user wants to update customer info or add a transaction
        cout << "Would you like to: " << endl;
        cout << "1 - Update Customer Information" << endl;
        cout << "2 - Add a Transaction (Update Total Sales)" << endl;
        cout << "3 - Cancel update" << endl;

        int choice;
        cin >> choice;
        cin.ignore(); // To clear the input buffer

        switch (choice) {
            case 1:  // Update Customer Information (without total sales)
                cout << "Which column would you like to update?" << endl;
                cout << "1 - Name" << endl;
                cout << "2 - City" << endl;
                cout << "3 - State" << endl;
                cout << "4 - Last Visit" << endl;
                cout << "5 - Cancel update" << endl;

                int info_choice;
                cin >> info_choice;
                cin.ignore(); // To clear the input buffer

                switch (info_choice) {
                    case 1:  // Update Name
                        cout << "Enter updated name (Currently: " << search->second.name << "): ";
                        getline(cin, name);
                        search->second.name = name;
                        break;
                    case 2:  // Update City
                        cout << "Enter updated city (Currently: " << search->second.city << "): ";
                        getline(cin, city);
                        search->second.city = city;
                        break;
                    case 3:  // Update State
                        cout << "Enter updated state (Currently: " << search->second.state << "): ";
                        getline(cin, state);
                        search->second.state = state;
                        break;
                    case 4:  // Update Last Visit
                        cout << "Enter updated last visit (YYYYMMDD) (Currently: " << search->second.last_visit << "): ";
                        cin >> last_visit;
                        search->second.last_visit = last_visit;
                        break;
                    case 5:  // Cancel Update
                        cout << "Update cancelled." << endl;
                        return;
                    default:
                        cout << "Invalid option, no changes made." << endl;
                        return;
                }
                cout << "Customer " << search->first << " updated." << endl;
                break;

            case 2:  // Add a Transaction (Update Total Sales)
                cout << "Enter the amount of the transaction to add to total sales: ";
                cin >> total_sales;
                search->second.total_sales += total_sales;  // Update total sales
                cout << "Transaction added. Updated Total Sales: $" << search->second.total_sales << endl;
                break;

            case 3:  // Cancel Update
                cout << "Update cancelled." << endl;
                return;

            default:
                cout << "Invalid option, no changes made." << endl;
                return;
        }
    } else {
        cout << "No customer ID " +  std::to_string(update_id) + " exists." << endl;
    }
}


// Function that handles showing options menu to user
// In interface.hpp, update the show_options method to include the search option:

void interface::show_options() {
    cout << "What would you like to do?" << endl;
    cout << "1 - Add Customer" << endl
         << "2 - Update Customer" << endl
         << "3 - Show Customers Table" << endl
         << "4 - Delete Customer" << endl
         << "5 - Show total sales" << endl
         << "6 - Search Customer by Name" << endl  // New search option
         << "7 - Exit Program" << endl;
}



// Function that handles showing user interface
void interface::show_interface() {
    customer_table.print_table(customer_table.get_max_id());
    int choice;
    // Run until user quits
    // In interface.hpp, update the main loop to handle the new option:

do {
    show_options();
    cin >> choice;

    if (choice == 1) {
        handle_add_cust();
    } else if (choice == 2) {
        handle_update_cust();
    } else if (choice == 3) {
        cout << "Select number of customers to show (Enter '*' to show all): " << endl;
        string n_show;
        cin >> n_show;
        if (n_show == "*") {
            customer_table.print_table(customer_table.get_max_id());
        } else {
            customer_table.print_table(stoi(n_show));
        }
    } else if (choice == 4) {
        handle_delete_cust();
    } else if (choice == 5) {
        cout << "Total company sales are: $"
             << customer_table.get_total_sales() << endl;
    } else if (choice == 6) { // New search customer option
        search_customer();
    } else if (choice == 7) {  // Quit program
        continue;
    } else {
        cout << "Please enter a number listed from the menu options (1-7)" << endl; 
    }
} while (choice != 7);

}

void interface::search_customer() {
    string search_name;
    cout << "Enter the customer name to search for: ";
    cin.ignore();
    getline(cin, search_name);

    bool found = false;
    // Iterate through the customer table to find the name
    for (auto it = customer_table.hashtable.begin(); it != customer_table.hashtable.end(); ++it) {
        if (it->second.name == search_name) {
            cout << "Customer Found!" << endl;
            cout << "ID: " << it->first << endl;
            cout << "Name: " << it->second.name << endl;
            cout << "City: " << it->second.city << endl;
            cout << "State: " << it->second.state << endl;
            cout << "Last Visit: " << it->second.format_date() << endl;
            cout << "Total Sales: $" << it->second.total_sales << endl;
            found = true;
            break;
        }
    }

    if (!found) {
        cout << "Customer with the name '" << search_name << "' not found." << endl;
    }
}