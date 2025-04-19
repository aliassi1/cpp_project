#include "interface.h"
#include <limits>
#include <iomanip>
#include <regex>
#include <stdexcept>

// Input validation functions
bool is_valid_name(const std::string& name) {
    if (name.empty()) {
        std::cout << "Name cannot be empty.\n";
        return false;
    }
    // Allow letters, spaces, and basic punctuation
    std::regex name_pattern("^[a-zA-Z\\s\\-']+$");
    if (!std::regex_match(name, name_pattern)) {
        std::cout << "Name can only contain letters, spaces, hyphens, and apostrophes.\n";
        return false;
    }
    return true;
}

bool is_valid_phone(const std::string& phone) {
    if (phone.empty()) {
        std::cout << "Phone number cannot be empty.\n";
        return false;
    }
    // Allow digits, spaces, hyphens, and parentheses
    std::regex phone_pattern("^[0-9\\s\\-()]+$");
    if (!std::regex_match(phone, phone_pattern)) {
        std::cout << "Phone number can only contain digits, spaces, hyphens, and parentheses.\n";
        return false;
    }
    return true;
}

bool is_valid_city(const std::string& city) {
    if (city.empty()) {
        std::cout << "City cannot be empty.\n";
        return false;
    }
    // Allow letters, spaces, and hyphens
    std::regex city_pattern("^[a-zA-Z\\s\\-]+$");
    if (!std::regex_match(city, city_pattern)) {
        std::cout << "City can only contain letters, spaces, and hyphens.\n";
        return false;
    }
    return true;
}

bool is_valid_date(const std::string& date) {
    if (date.empty()) {
        std::cout << "Date cannot be empty.\n";
        return false;
    }
    // Check YYYY-MM-DD format
    std::regex date_pattern("^\\d{4}-(?:0[1-9]|1[0-2])-(?:0[1-9]|[12]\\d|3[01])$");
    if (!std::regex_match(date, date_pattern)) {
        std::cout << "Date must be in YYYY-MM-DD format.\n";
        return false;
    }
    return true;
}

bool is_valid_status(const std::string& status) {
    if (status.empty()) {
        std::cout << "Status cannot be empty.\n";
        return false;
    }
    return (status == "Active" || status == "Inactive" || status == "Paused");
}

// Add a new customer
void interface::handle_add_cust() {
    try {
        std::string name, phone, city, expiry_date, status;
        int sessions_purchased;
        float total_paid;
        int last_id = customer_table.get_max_id();

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        // Get and validate name
        do {
            std::cout << "Enter member full name: ";
            std::getline(std::cin, name);
        } while (!is_valid_name(name));

        // Get and validate phone
        do {
            std::cout << "Enter phone number: ";
            std::getline(std::cin, phone);
        } while (!is_valid_phone(phone));

        // Get and validate city
        do {
            std::cout << "Enter city: ";
            std::getline(std::cin, city);
        } while (!is_valid_city(city));

        // Get and validate expiry date
        do {
            std::cout << "Enter membership expiry date (YYYY-MM-DD): ";
            std::getline(std::cin, expiry_date);
        } while (!is_valid_date(expiry_date));

        // Get and validate sessions purchased
        do {
            std::cout << "Enter number of sessions purchased: ";
            if (!(std::cin >> sessions_purchased) || sessions_purchased <= 0) {
                std::cout << "Sessions purchased must be a positive number.\n";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                continue;
            }
            break;
        } while (true);

        std::cin.ignore();

        // Get and validate status
        do {
            std::cout << "Enter membership status (Active/Inactive/Paused): ";
            std::getline(std::cin, status);
        } while (!is_valid_status(status));

        // Get and validate total paid
        do {
            std::cout << "Enter total money paid: $";
            if (!(std::cin >> total_paid) || total_paid < 0) {
                std::cout << "Total paid must be a non-negative number.\n";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                continue;
            }
            break;
        } while (true);

        customer new_cust(last_id + 1, name, phone, city, expiry_date,
                         sessions_purchased, status, total_paid);
        customer_table.insert_row(last_id + 1, new_cust);
        customer_table.write_data();
        std::cout << "✅ Member added successfully.\n";
    } catch (const std::exception& e) {
        std::cerr << "Error adding member: " << e.what() << std::endl;
    }
}

// Delete a customer
void interface::handle_delete_cust() {
    try {
        int delete_id;
        std::cout << "Enter customer ID to delete: ";
        if (!(std::cin >> delete_id) || delete_id <= 0) {
            std::cout << "❌ Invalid customer ID.\n";
            return;
        }

        auto search = customer_table.hashtable.find(delete_id);
        if (search != customer_table.hashtable.end()) {
            const customer& c = search->second;
            std::cout << "Found member:\n";
            std::cout << "Name: " << c.name << "\n";
            std::cout << "Phone: " << c.phone << "\n";
            std::cout << "City: " << c.city << "\n";
            std::cout << "Expiry: " << c.expiry_date << "\n";
            std::cout << "Sessions Purchased: " << c.sessions_purchased << "\n";
            std::cout << "Sessions Used: " << c.sessions_used << "\n";
            std::cout << "Status: " << c.status << "\n";
            std::cout << "Total Paid: $" << c.total_paid << "\n";

            std::string confirm;
            do {
                std::cout << "Are you sure you want to delete this member? (Y/N): ";
                std::cin >> confirm;
            } while (confirm != "Y" && confirm != "y" && confirm != "N" && confirm != "n");

            if (confirm == "Y" || confirm == "y") {
                customer_table.hashtable.erase(delete_id);
                customer_table.write_data();
                std::cout << "✅ Member deleted.\n";
            } else {
                std::cout << "Deletion cancelled.\n";
            }
        } else {
            std::cout << "❌ No member with ID " << delete_id << " found.\n";
        }
    } catch (const std::exception& e) {
        std::cerr << "Error deleting member: " << e.what() << std::endl;
    }
}

// Update a customer
void interface::handle_update_cust() {
    try {
        int update_id;
        std::cout << "Enter customer ID to update: ";
        if (!(std::cin >> update_id) || update_id <= 0) {
            std::cout << "❌ Invalid customer ID.\n";
            return;
        }

        auto search = customer_table.hashtable.find(update_id);
        if (search != customer_table.hashtable.end()) {
            std::string name, phone, city, expiry_date, status;
            int sessions_purchased;
            double total_paid;

            std::cout << "Found member ID: " << update_id << "\n";
            std::cout << "Current member information:\n";
            std::cout << "Name: " << search->second.name << "\n";
            std::cout << "Phone: " << search->second.phone << "\n";
            std::cout << "City: " << search->second.city << "\n";
            std::cout << "Expiry Date: " << search->second.expiry_date << "\n";
            std::cout << "Status: " << search->second.status << "\n";
            std::cout << "Sessions Purchased: " << search->second.sessions_purchased << "\n";
            std::cout << "Sessions Used: " << search->second.sessions_used << "\n";
            std::cout << "Total Paid: $" << search->second.total_paid << "\n\n";

            std::cout << "Select update option:\n";
            std::cout << "1 - Update Member Information (name, phone, city, expiry date, status)\n";
            std::cout << "2 - Update Sessions and Payment (sessions purchased, total paid)\n";
            std::cout << "3 - Cancel\n";
            std::cout << "Enter your choice (1-3): ";

            int choice;
            if (!(std::cin >> choice) || choice < 1 || choice > 3) {
                std::cout << "❌ Invalid choice. Update cancelled.\n";
                return;
            }
            std::cin.ignore();

            if (choice == 1) {
                std::cout << "\nUpdate Member Information:\n";
                
                // Update name
                do {
                    std::cout << "Enter new name (press Enter to keep current): ";
                    std::getline(std::cin, name);
                    if (name.empty()) break;
                } while (!is_valid_name(name));
                if (!name.empty()) search->second.name = name;

                // Update phone
                do {
                    std::cout << "Enter new phone (press Enter to keep current): ";
                    std::getline(std::cin, phone);
                    if (phone.empty()) break;
                } while (!is_valid_phone(phone));
                if (!phone.empty()) search->second.phone = phone;

                // Update city
                do {
                    std::cout << "Enter new city (press Enter to keep current): ";
                    std::getline(std::cin, city);
                    if (city.empty()) break;
                } while (!is_valid_city(city));
                if (!city.empty()) search->second.city = city;

                // Update expiry date
                do {
                    std::cout << "Enter new expiry date YYYY-MM-DD (press Enter to keep current): ";
                    std::getline(std::cin, expiry_date);
                    if (expiry_date.empty()) break;
                } while (!is_valid_date(expiry_date));
                if (!expiry_date.empty()) search->second.expiry_date = expiry_date;

                // Update status
                do {
                    std::cout << "Enter new status (Active/Inactive/Paused) (press Enter to keep current): ";
                    std::getline(std::cin, status);
                    if (status.empty()) break;
                } while (!is_valid_status(status));
                if (!status.empty()) search->second.status = status;

                customer_table.write_data();
                std::cout << "✅ Member information updated successfully.\n";

            } else if (choice == 2) {
                std::cout << "\nUpdate Sessions and Payment:\n";
                std::cout << "Current sessions purchased: " << search->second.sessions_purchased << "\n";
                
                // Update sessions purchased
                do {
                    std::cout << "Enter number of additional sessions to add: ";
                    if (!(std::cin >> sessions_purchased) || sessions_purchased <= 0) {
                        std::cout << "Sessions must be a positive number.\n";
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        continue;
                    }
                    break;
                } while (true);

                search->second.sessions_purchased += sessions_purchased;
                std::cout << "New total sessions: " << search->second.sessions_purchased << "\n";

                // Update total paid
                do {
                    std::cout << "Enter payment amount for the new sessions: $";
                    if (!(std::cin >> total_paid) || total_paid < 0) {
                        std::cout << "Payment amount must be a non-negative number.\n";
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        continue;
                    }
                    break;
                } while (true);

                search->second.total_paid += total_paid;
                customer_table.write_data();
                std::cout << "✅ Sessions and payment updated successfully.\n";
                std::cout << "New total paid: $" << search->second.total_paid << "\n";

            } else if (choice == 3) {
                std::cout << "Update cancelled.\n";
                return;
            }

        } else {
            std::cout << "❌ No member with ID " << update_id << " found.\n";
        }
    } catch (const std::exception& e) {
        std::cerr << "Error updating member: " << e.what() << std::endl;
    }
}

// Show admin options
void interface::show_options() {

    cout << "\n+===============================================================+" << endl;
    cout << "|                    Fitness CUSTOMER MANAGEMENT SYSTEM           |" << endl;
    cout << "+===============================================================+" << endl;
    cout << "|                                                               |" << endl;
    cout << "|  [1] > Add New Customer                                      |" << endl;
    cout << "|  [2] > Update Customer Information                           |" << endl;
    cout << "|  [3] > Display Customer Table                               |" << endl;
    cout << "|  [4] > Delete Customer                                       |" << endl;
    cout << "|  [5] > View Total Sales                                      |" << endl;
    cout << "|  [6] > Search Customer by Name                              |" << endl;
    cout << "|  [7] > Exit Program                                          |" << endl;
    cout << "|                                                               |" << endl;
    cout << "+===============================================================+" << endl;
    cout << "\nEnter your choice (1-7): ";
}

// Admin interface loop
void interface::show_interface() {
    cout << "\n+===============================================================+" << endl;
    cout << "|                    WELCOME TO CUSTOMER MANAGEMENT             |" << endl;
    cout << "+===============================================================+\n" << endl;
    
    customer_table.print_table(customer_table.get_max_id());
    int choice;

    
    do {
        show_options();
        cin >> choice;

        if (choice == 1) {
            cout << "\n+===============================================================+" << endl;
            cout << "|                    ADD NEW CUSTOMER                            |" << endl;
            cout << "+===============================================================+\n" << endl;
            handle_add_cust();
        } else if (choice == 2) {
            cout << "\n+===============================================================+" << endl;
            cout << "|                    UPDATE CUSTOMER                             |" << endl;
            cout << "+===============================================================+\n" << endl;
            handle_update_cust();
        } else if (choice == 3) {
            cout << "\n+===============================================================+" << endl;
            cout << "|                    DISPLAY CUSTOMERS                           |" << endl;
            cout << "+===============================================================+\n" << endl;
            cout << "Select number of customers to show (Enter '*' to show all): ";
            string n_show;
            cin >> n_show;
            if (n_show == "*") {
                customer_table.print_table(customer_table.get_max_id());
            } else {
                customer_table.print_table(stoi(n_show));
            }
        } else if (choice == 4) {
            cout << "\n+===============================================================+" << endl;
            cout << "|                    DELETE CUSTOMER                             |" << endl;
            cout << "+===============================================================+\n" << endl;
            handle_delete_cust();
        } else if (choice == 5) {
            cout << "\n+===============================================================+" << endl;
            cout << "|                    TOTAL SALES REPORT                          |" << endl;
            cout << "+===============================================================+\n" << endl;
            cout << "Total company sales are: $" << customer_table.get_total_paid() << endl;
        } else if (choice == 6) {
            cout << "\n+===============================================================+" << endl;
            cout << "|                    SEARCH CUSTOMER                             |" << endl;
            cout << "+===============================================================+\n" << endl;
            search_customer();
        } else if (choice == 7) {
            cout << "\n+===============================================================+" << endl;
            cout << "|                    THANK YOU FOR USING                         |" << endl;
            cout << "|                    CUSTOMER MANAGEMENT SYSTEM                   |" << endl;
            cout << "+===============================================================+\n" << endl;
            continue;
        } else {
            cout << "\n[!] ERROR: Please enter a valid number between 1 and 7 [!]" << endl;
        }
    } while (choice != 7);
}

// Search member by name
void interface::search_customer() {
    std::string search_name;
    std::cout << "Enter member name to search: ";
    std::cin.ignore();
    std::getline(std::cin, search_name);

    bool found = false;

    cout << "\n+===============================================================+" << endl;
    cout << "|                    SEARCH RESULTS                              |" << endl;
    cout << "+===============================================================+\n" << endl;

    for (auto it = customer_table.hashtable.begin(); it != customer_table.hashtable.end(); ++it) {
        if (it->second.name == search_name) {
            cout << "[+] Customer Found!" << endl;
            cout << "-----------------------------------------------------------------" << endl;
            cout << "ID: " << it->first << endl;
            cout << "Name: " << it->second.name << endl;
            cout << "City: " << it->second.city << endl;
            cout << "Status: " << it->second.status << endl;
            cout << "Last Visit: " << it->second.format_date() << endl;
            cout << "Total Paid: $" << it->second.total_paid << endl;
            cout << "-----------------------------------------------------------------" << endl;
            found = true;
            break;
        }
    }

    if (!found) {

        cout << "[-] Customer with the name '" << search_name << "' not found." << endl;
    }
}

// Member-only view
void interface::show_member_view(const std::string& username) {
    std::cout << "\n==== Member Dashboard ====\n";

    bool found = false;
    for (const auto& [id, m] : customer_table.hashtable) {
        if (m.name == username) {
            std::cout << "Member ID: " << m.id << "\n"
                      << "Name: " << m.name << "\n"
                      << "Phone: " << m.phone << "\n"
                      << "City: " << m.city << "\n"
                      << "Expiry Date: " << m.expiry_date << "\n"
                      << "Sessions Purchased: " << m.sessions_purchased << "\n"
                      << "Sessions Used: " << m.sessions_used << "\n"
                      << "Status: " << m.status << "\n"
                      << "Total Paid: $" << m.total_paid << "\n";
            found = true;
            break;
        }
    }

    if (!found) {
        std::cout << "❌ No data found for member: " << username << "\n";
    }
}

// Check-in: use session
void interface::use_session() {
    int id;
    std::cout << "Enter member ID to check in: ";
    std::cin >> id;

    auto it = customer_table.hashtable.find(id);
    if (it != customer_table.hashtable.end()) {
        if (it->second.sessions_used < it->second.sessions_purchased) {
            it->second.sessions_used++;
            std::cout << "✅ Check-in complete. "
                      << "Used " << it->second.sessions_used
                      << " / Purchased " << it->second.sessions_purchased << "\n";
            customer_table.write_data();
        } else {
            std::cout << "❌ No sessions remaining. Member must purchase more sessions.\n";
        }
    } else {
        std::cout << "❌ Member ID not found.\n";
    }
}
