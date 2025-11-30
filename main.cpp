#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <limits> // added for input clearing
#include <iomanip> // added for table formatting
using namespace std;

struct Item {
    string name;
    string ID;
    int price;
    int quantity;
    Item* next;
};

class Inventory {
private:
    Item* head;

public:
    // Constructor , auto head to nullptr and load data from CSV
    Inventory() : head(nullptr) {
        loadFromCSV();  // Load data when inventory is created
    }

    ~Inventory() {
        while (head) {
            Item* temp = head;
            head = head->next;
            delete temp;
        }
    }

    // ----------------- ADD ITEM -----------------
    void addItem(const string& name, const string& ID, int price, int quantity) {
        Item* newItem = new Item{name, ID, price, quantity, nullptr};

        // Add to linked list
        if (!head) {
            head = newItem;
        } else {
            Item* temp = head;
            while (temp->next)
                temp = temp->next;
            temp->next = newItem;
        }

        // Append to CSV
        ofstream file("inventory.csv", ios::app);
        if (file.is_open()) {
            file << name << "," << ID << "," << price << "," << quantity << "\n";
            file.close();
            cout << "Item added and saved to inventory.csv!\n";
        } else {
            cout << "Error opening file!\n";
        }
    }

    // ----------------- REMOVE ITEM -----------------
    bool removeItem(const string& ID) {
        // Remove from linked list first
        bool removed = false;
        Item* curr = head;
        Item* prev = nullptr;

        while (curr) {
            if (curr->ID == ID) {
                if (!prev) head = curr->next;
                else prev->next = curr->next;
                delete curr;
                removed = true;
                break;
            }
            prev = curr;
            curr = curr->next;
        }

        if (!removed) return false;

        // Rebuild CSV without removed item
        ofstream tempFile("temp.csv");
        ifstream file("inventory.csv");
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string name, id, price, quantity;
            getline(ss, name, ',');
            getline(ss, id, ',');
            getline(ss, price, ',');
            getline(ss, quantity, ',');
            if (id != ID) {
                tempFile << name << "," << id << "," << price << "," << quantity << "\n";
            }
        }
        file.close();
        tempFile.close();

        // Replace original file
        remove("inventory.csv");
        rename("temp.csv", "inventory.csv");

        return true;
    }

    // ----------------- DISPLAY ITEMS -----------------
    void display() const {
        cout << "\n";
        if (!head) {
            cout << "Inventory is empty.\n";
            return;
        }

        // Compact column widths for cleaner UX
        const int nameW  = 18;
        const int idW    = 8;
        const int priceW = 8;
        const int qtyW   = 5;

        // Top border + header
        cout << "+" << string(nameW, '-') << "+" << string(idW, '-') << "+" << string(priceW, '-') << "+" << string(qtyW, '-') << "+\n";
        cout << "| " << left  << setw(nameW - 1)  << "Name"
             << "| " << left  << setw(idW - 1)    << "ID"
             << "| " << right << setw(priceW - 1) << "Price"
             << "| " << right << setw(qtyW - 1)   << "Qty" << "|\n";
        cout << "+" << string(nameW, '-') << "+" << string(idW, '-') << "+" << string(priceW, '-') << "+" << string(qtyW, '-') << "+\n";

        // Rows
        Item* temp = head;
        while (temp) {
            string displayName = temp->name;
            if ((int)displayName.size() > nameW - 1)
                displayName = displayName.substr(0, nameW - 4) + "...";

            cout << "| " << left  << setw(nameW - 1)  << displayName
                 << "| " << left  << setw(idW - 1)    << temp->ID
                 << "| " << right << setw(priceW - 1) << temp->price
                 << "| " << right << setw(qtyW - 1)   << temp->quantity << "|\n";
            temp = temp->next;
        }

        // Bottom border
        cout << "+" << string(nameW, '-') << "+" << string(idW, '-') << "+" << string(priceW, '-') << "+" << string(qtyW, '-') << "+\n";
    }

    // ----------------- LOAD FROM CSV -----------------
    void loadFromCSV() {
        ifstream file("inventory.csv");
        if (!file.is_open()) {
            return;  // File doesn't exist yet
        }

        string line;
        bool isHeader = true;

        while (getline(file, line)) {
            if (isHeader) {
                isHeader = false;  // Skip first line (header)
                continue;
            }

            stringstream ss(line);
            string name, ID, priceStr, quantityStr;
            
            getline(ss, name, ',');
            getline(ss, ID, ',');
            getline(ss, priceStr, ',');
            getline(ss, quantityStr, ',');

            int price = stoi(priceStr);
            int quantity = stoi(quantityStr);

            // Add to linked list without writing to CSV again
            Item* newItem = new Item{name, ID, price, quantity, nullptr};
            if (!head) {
                head = newItem;
            } else {
                Item* temp = head;
                while (temp->next)
                    temp = temp->next;
                temp->next = newItem;
            }
        }
        file.close();
    }
};

int main() {
    Inventory inv;
    int mainChoice;

    while (true) {
        cout << "\n==== INVENTORY MENU ====\n";
        cout << "1. Add/Remove Item\n";
        cout << "2. Show Inventory\n";
        cout << "3. Exit\n";
        cout << "Choose: ";
        if (!(cin >> mainChoice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input.\n";
            continue;
        }

        if (mainChoice == 1) {
            int subChoice;
            while (true) {
                cout << "\n-- Add/Remove Menu --\n";
                cout << "1. Add Item\n";
                cout << "2. Remove Item\n";
                cout << "3. Back\n";
                cout << "Choose: ";
                if (!(cin >> subChoice)) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid input.\n";
                    continue;
                }

                if (subChoice == 1) {
                    string name, ID;
                    int price, quantity;
                    cout << "Enter name: ";
                    cin >> name;
                    cout << "Enter ID: ";
                    cin >> ID;
                    cout << "Enter price: ";
                    cin >> price;
                    cout << "Enter quantity: ";
                    cin >> quantity;
                    inv.addItem(name, ID, price, quantity);
                } else if (subChoice == 2) {
                    string ID;
                    cout << "Enter ID to remove: ";
                    cin >> ID;
                    if (inv.removeItem(ID))
                        cout << "Item removed successfully from CSV!\n";
                    else
                        cout << "Item not found.\n";
                } else if (subChoice == 3) {
                    break; // return to main menu
                } else {
                    cout << "Invalid option.\n";
                }
            }
        }
        else if (mainChoice == 2) {
            inv.display();
        }
        else if (mainChoice == 3) {
            cout << "Exiting program. Goodbye!\n";
            break;
        }
        else {
            cout << "Invalid option.\n";
        }
    }

    return 0;
}

