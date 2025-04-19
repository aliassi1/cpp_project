#include "interface.h"
#include <limits>
#include <iomanip>

// Add a new customer
void interface::handle_add_cust() {
    std::string name, phone, city, expiry_date, status;
    int sessions_purchased;
    float total_paid;
    int last_id = customer_table.get_max_id();

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Enter member full name: ";
    std::getline(std::cin, name);
    std::cout << "Enter phone number: ";
    std::getline(std::cin, phone);
    std::cout << "Enter city: ";
    std::getline(std::cin, city);
    std::cout << "Enter membership expiry date (YYYY-MM-DD): ";
    std::getline(std::cin, expiry_date);
    std::cout << "Enter number of sessions purchased: ";
    std::cin >> sessions_purchased;
    std::cin.ignore();
    std::cout << "Enter membership status (Active/Inactive/Paused): ";
    std::getline(std::cin, status);
    std::cout << "Enter total money paid: $";
    std::cin >> total_paid;

    customer new_cust(last_id + 1, name, phone, city, expiry_date,
                      sessions_purchased, status, total_paid);
    customer_table.insert_row(last_id + 1, new_cust);
    std::cout << "✅ Member added successfully.\n";
}

// Delete a customer
void interface::handle_delete_cust() {
    int delete_id;
    std::cout << "Enter customer ID to delete: ";
    std::cin >> delete_id;

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
        std::cout << "Are you sure you want to delete this member? (Y/N): ";
        std::cin >> confirm;
        if (confirm == "Y" || confirm == "y") {
            customer_table.hashtable.erase(delete_id);
            std::cout << "✅ Member deleted.\n";
        } else {
            std::cout << "Deletion cancelled.\n";
        }
    } else {
        std::cout << "❌ No member with ID " << delete_id << " found.\n";
    }
}

// Update a customer
void interface::handle_update_cust() {
    int update_id;
    std::cout << "Enter customer ID to update: ";
    std::cin >> update_id;

    auto search = customer_table.hashtable.find(update_id);
    if (search != customer_table.hashtable.end()) {
        std::string name, phone, city, expiry_date, status;
        int sessions_purchased;
        float transaction;

        std::cout << "Found member ID: " << update_id << "\n";

        std::cout << "1 - Update Member Information\n";
        std::cout << "2 - Add Payment\n";
        std::cout << "3 - Cancel\n";

        int choice;
        std::cin >> choice;
        std::cin.ignore();

        if (choice == 1) {
            std::cout << "Select field to update:\n";
            std::cout << "1 - Name\n2 - Phone\n3 - City\n4 - Expiry Date\n5 - Status\n6 - Sessions Purchased\n7 - Cancel\n";

            int field;
            std::cin >> field;
            std::cin.ignore();

            switch (field) {
                case 1:
                    std::cout << "New Name: ";
                    std::getline(std::cin, name);
                    search->second.name = name;
                    break;
                case 2:
                    std::cout << "New Phone: ";
                    std::getline(std::cin, phone);
                    search->second.phone = phone;
                    break;
                case 3:
                    std::cout << "New City: ";
                    std::getline(std::cin, city);
                    search->second.city = city;
                    break;
                case 4:
                    std::cout << "New Expiry Date (YYYY-MM-DD): ";
                    std::getline(std::cin, expiry_date);
                    search->second.expiry_date = expiry_date;
                    break;
                case 5:
                    std::cout << "New Status: ";
                    std::getline(std::cin, status);
                    search->second.status = status;
                    break;
                case 6:
                    std::cout << "Enter updated number of sessions purchased: ";
                    std::cin >> sessions_purchased;
                    search->second.sessions_purchased = sessions_purchased;
                    break;
                default:
                    std::cout << "Update cancelled.\n";
                    return;
            }
            std::cout << "✅ Member updated.\n";

        } else if (choice == 2) {
            std::cout << "Add amount paid: $";
            std::cin >> transaction;
            search->second.total_paid += transaction;
            std::cout << "✅ Payment recorded. Total Paid: $" << search->second.total_paid << "\n";
        } else {
            std::cout << "Update cancelled.\n";
        }

    } else {
        std::cout << "❌ No customer with ID " << update_id << " found.\n";
    }
}

// Show admin options
void interface::show_options() {
    std::cout << "\nWhat would you like to do?\n";
    std::cout << "1 - Add Member\n"
              << "2 - Update Member\n"
              << "3 - Show All Members\n"
              << "4 - Delete Member\n"
              << "5 - Show Total Revenue\n"
              << "6 - Search Member by Name\n"
              << "7 - Log Member Check-in\n"
              << "8 - Exit\n";
}

// Admin interface loop
void interface::show_interface() {
    customer_table.print_table(customer_table.get_max_id());
    int choice;
    do {
        show_options();
        std::cin >> choice;

        if (choice == 1) handle_add_cust();
        else if (choice == 2) handle_update_cust();
        else if (choice == 3) {
            std::string input;
            std::cout << "How many members to show? (Enter '*' for all): ";
            std::cin >> input;
            if (input == "*") customer_table.print_table(customer_table.get_max_id());
            else customer_table.print_table(std::stoi(input));
        }
        else if (choice == 4) handle_delete_cust();
        else if (choice == 5) {
            std::cout << "Total revenue from members: $"
                      << customer_table.get_total_paid() << "\n";
        }
        else if (choice == 6) search_customer();
        else if (choice == 7) use_session();
        else if (choice != 8) {
            std::cout << "Invalid choice.\n";
        }
    } while (choice != 8);
}

// Search member by name
void interface::search_customer() {
    std::string search_name;
    std::cout << "Enter member name to search: ";
    std::cin.ignore();
    std::getline(std::cin, search_name);

    bool found = false;
    for (const auto& [id, c] : customer_table.hashtable) {
        if (c.name == search_name) {
            std::cout << "\nMember Found:\n"
                      << "ID: " << c.id << "\n"
                      << "Name: " << c.name << "\n"
                      << "Phone: " << c.phone << "\n"
                      << "City: " << c.city << "\n"
                      << "Expiry: " << c.expiry_date << "\n"
                      << "Sessions Purchased: " << c.sessions_purchased << "\n"
                      << "Sessions Used: " << c.sessions_used << "\n"
                      << "Status: " << c.status << "\n"
                      << "Total Paid: $" << c.total_paid << "\n";
            found = true;
            break;
        }
    }

    if (!found) {
        std::cout << "❌ Member not found.\n";
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
