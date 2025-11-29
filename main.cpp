#include <iostream>
#include <fstream>
#include <string>
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
    Inventory() {
        head = nullptr;
    }

    // ----------------- ADD ITEM -----------------
    void addItem(string name, string ID, int price, int quantity) {
        Item* newItem = new Item;
        newItem->name = name;
        newItem->ID = ID;
        newItem->price = price;
        newItem->quantity = quantity;
        newItem->next = nullptr;

        if (head == nullptr) {
            head = newItem;
        } 
        else {
            Item* temp = head;
            while (temp->next != nullptr) 
                temp = temp->next;
            temp->next = newItem;
        }

        cout << "✔ Item added successfully!\n";
    }

    // ----------------- REMOVE ITEM -----------------
    bool removeItem(string ID) {
        if (head == nullptr) return false;

        // Remove first item
        if (head->ID == ID) {
            Item* temp = head;
            head = head->next;
            delete temp;
            return true;
        }

        Item* curr = head;
        while (curr->next != nullptr && curr->next->ID != ID)
            curr = curr->next;

        if (curr->next == nullptr) return false;

        Item* deleteNode = curr->next;
        curr->next = curr->next->next;
        delete deleteNode;

        return true;
    }

    // ----------------- SAVE TO FILE -----------------
    void saveToFile() {
        ofstream file("inventory.txt");

        Item* temp = head;
        while (temp != nullptr) {
            file << temp->name << ","
                 << temp->ID << ","
                 << temp->price << ","
                 << temp->quantity << "\n";
            temp = temp->next;
        }

        file.close();
        cout << "Inventory saved to file.\n";
    }

    // ----------------- DISPLAY ITEMS -----------------
    void display() {
        Item* temp = head;
        while (temp != nullptr) {
            cout << "Name: " << temp->name
                 << " | ID: " << temp->ID
                 << " | Price: " << temp->price
                 << " | Quantity: " << temp->quantity
                 << endl;
            temp = temp->next;
        }
    }
};

int main() {
    Inventory inv;
    int choice;

    while (true) {
        cout << "\n==== INVENTORY MENU ====\n";
        cout << "1. Add Item\n";
        cout << "2. Remove Item\n";
        cout << "3. Show Inventory\n";
        cout << "4. Save & Exit\n";
        cout << "Choose: ";
        cin >> choice;

        if (choice == 1) {
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
        }
        else if (choice == 2) {
            string ID;
            cout << "Enter ID to remove: ";
            cin >> ID;

            if (inv.removeItem(ID))
                cout << "✔ Item removed.\n";
            else
                cout << "❌ Item not found.\n";
        }
        else if (choice == 3) {
            inv.display();
        }
        else if (choice == 4) {
            inv.saveToFile();
            break;
        }
        else {
            cout << "Invalid option.\n";
        }
    }

    return 0;
}
