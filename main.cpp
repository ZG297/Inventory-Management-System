#include <iostream>
#include <string>
#include <cstdlib>
#include "Product.h"
#include "ProductManager.h"
#include "Transaction.h"
#include "TransactionList.h"
using namespace std;

// ---------- limits ----------
const int MAX_ID = 999999;
const int MAX_QTY = 1000000;
const int MAX_NAME = 19;
const double MAX_PRICE = 1000000;

ProductManager pm;
TransactionList tl;

// ---------- input helpers ----------

// reads one line and removes spaces from both ends
string readLine(string prompt) {
    string s;
    cout << prompt;
    if (!getline(cin, s)) {
        cout << "\nInput closed. Exiting.\n";
        exit(0);
    }
    while (!s.empty() && s[0] == ' ') s.erase(0, 1);
    while (!s.empty() && s[s.size() - 1] == ' ') s.erase(s.size() - 1);
    return s;
}

// reads a whole number between min and max 
int readInt(string prompt, int min, int max) {
    while (true) {
        string s = readLine(prompt);

        bool ok = !s.empty() && s.size() <= 9;
        for (size_t i = 0; i < s.size(); i++) {
            if (s[i] < '0' || s[i] > '9') ok = false;
        }
        if (!ok) {
            cout << "Error: enter a valid whole number (digits only).\n";
            continue;
        }

        int n = stoi(s);
        if (n < min || n > max) {
            cout << "Error: number must be between " << min << " and " << max << ".\n";
            continue;
        }
        return n;
    }
}

// reads a price 
double readPrice(string prompt) {
    while (true) {
        string s = readLine(prompt);

        int dots = 0, digits = 0, decimals = 0;
        bool ok = !s.empty() && s.size() <= 10;
        for (size_t i = 0; i < s.size(); i++) {
            if (s[i] == '.') dots++;
            else if (s[i] >= '0' && s[i] <= '9') {
                digits++;
                if (dots == 1) decimals++;
            }
            else ok = false;
        }
        if (!ok || dots > 1 || digits == 0) {
            cout << "Error: enter a valid price like 250 or 99.50.\n";
            continue;
        }
        if (decimals > 2) {
            cout << "Error: only 2 digits allowed after the decimal point.\n";
            continue;
        }

        double p = stod(s);
        if (p < 0.01 || p > MAX_PRICE) {
            cout << "Error: price must be between 0.01 and 1000000.\n";
            continue;
        }
        return p;
    }
}

// reads a product name
string readName(string prompt) {
    while (true) {
        string s = readLine(prompt);

        if (s.empty())
            cout << "Error: name cannot be empty.\n";
        else if ((int)s.size() > MAX_NAME)
            cout << "Error: name can have at most 19 characters.\n";
        else if (s.find('|') != string::npos)
            cout << "Error: name cannot contain the '|' character.\n";
        else
            return s;
    }
}

bool readYesNo(string prompt) {
    while (true) {
        string s = readLine(prompt + " (y/n): ");
        if (s == "y" || s == "Y") return true;
        if (s == "n" || s == "N") return false;
        cout << "Error: type y or n.\n";
    }
}

// ---------- small helpers ----------
void saveAll() {
    pm.saveToFile();
    tl.saveToFile();
}

void showProduct(Product p) {
    Product::printHeader();
    p.display();
}

// ---------- menu functions ----------
void addProduct() {
    cout << "\n--- Add Product ---\n";
    int id = readInt("Product ID : ", 1, MAX_ID);
    if (pm.isExistingId(id)) {
        cout << "Error: Product ID " << id << " already exists. Duplicate IDs are not allowed.\n";
        return;
    }

    string name = readName("Product name : ");
    if (pm.searchByName(name) != -1) {
        cout << "Error: a product with this name already exists.\n";
        return;
    }

    int qty = readInt("Quantity : ", 0, MAX_QTY);
    double price = readPrice("Price : ");
    int threshold = readInt("Low-stock threshold : ", 0, MAX_QTY);

    Product p(id, name, qty, price, threshold);
    if (pm.addProduct(p)) {
        saveAll();
        cout << "Product added successfully.\n";
    } else {
        cout << "Error: product could not be added.\n";
    }
}

void updateProduct() {
    cout << "\n--- Update Product ---\n";
    if (pm.isEmpty()) {
        cout << "No products in inventory.\n";
        return;
    }

    int id = readInt("Enter Product ID to update: ", 1, MAX_ID);
    int index = pm.searchById(id);
    if (index == -1) {
        cout << "Error: product not found.\n";
        return;
    }

    Product p = pm.getProductAt(index);
    showProduct(p);

    cout << "\nWhat do you want to update?\n"
         << " 1. Name\n"
         << " 2. Price\n"
         << " 3. Low-stock threshold\n"
         << " 4. Quantity\n"
         << " 0. Cancel\n";
    int choice = readInt("Choice: ", 0, 4);

    if (choice == 0) {
        cout << "Cancelled.\n";
        return;
    }

    if (choice == 1) {
        string name = readName("New name: ");
        int found = pm.searchByName(name);
        if (found != -1 && found != index) {
            cout << "Error: another product already uses this name.\n";
            return;
        }
        p.setProductName(name);
    }
    else if (choice == 2) {
        p.setPrice(readPrice("New price : "));
    }
    else if (choice == 3) {
        p.setThreshold(readInt("New threshold : ", 0, MAX_QTY));
    }
    else {
        p.setQuantity(readInt("New quantity : ", 0, MAX_QTY));
    }

    if (pm.updateProduct(p)) {
        saveAll();
        cout << "Product updated successfully.\n";
    } else {
        cout << "Error: update failed.\n";
    }
}

void deleteProduct() {
    cout << "\n--- Delete Product ---\n";
    if (pm.isEmpty()) {
        cout << "No products in inventory.\n";
        return;
    }

    int id = readInt("Enter Product ID to delete: ", 1, MAX_ID);
    int index = pm.searchById(id);
    if (index == -1) {
        cout << "Error: product not found.\n";
        return;
    }

    showProduct(pm.getProductAt(index));
    if (readYesNo("Are you sure you want to delete this product?")) {
        pm.deleteProduct(id);
        saveAll();
        cout << "Product deleted.\n";
    } else {
        cout << "Deletion cancelled.\n";
    }
}

void searchProduct() {
    cout << "\n--- Search Product ---\n";
    if (pm.isEmpty()) {
        cout << "No products in inventory.\n";
        return;
    }

    cout << " 1. Search by ID\n"
         << " 2. Search by exact name\n"
         << " 3. Search by name prefix (starts with)\n"
         << " 0. Back\n";
    int choice = readInt("Choice: ", 0, 3);

    if (choice == 1) {
        int id = readInt("Enter Product ID: ", 1, MAX_ID);
        int index = pm.searchById(id);
        if (index == -1) cout << "  Product not found.\n";
        else showProduct(pm.getProductAt(index));
    }
    else if (choice == 2) {
        string name = readName("Enter product name: ");
        int index = pm.searchByName(name);
        if (index == -1) cout << "  Product not found.\n";
        else showProduct(pm.getProductAt(index));
    }
    else if (choice == 3) {
        string prefix = readName("Enter starting letters: ");
        pm.searchByNamePrefix(prefix);
    }
}

void sortProducts() {
    cout << "\n--- Sort Inventory ---\n";
    if (pm.isEmpty()) {
        cout << "No products in inventory.\n";
        return;
    }

    cout << " 1. Sort by Product ID\n"
         << " 2. Sort by Name (A-Z)\n"
         << " 3. Sort by Quantity (low to high)\n"
         << " 0. Back\n";
    int choice = readInt("Choice: ", 0, 3);

    if (choice == 0) return;
    if (choice == 1) pm.sortById();
    if (choice == 2) pm.sortByName();
    if (choice == 3) pm.sortByQuantity();

    saveAll();
    cout << " Sorted successfully:\n";
    pm.displayAll();
}

void stockIn() {
    cout << "\n--- Stock In ---\n";
    if (pm.isEmpty()) {
        cout << " No products in inventory.\n";
        return;
    }

    int id = readInt("Enter Product ID: ", 1, MAX_ID);
    int index = pm.searchById(id);
    if (index == -1) {
        cout << "Error: product not found.\n";
        return;
    }

    Product p = pm.getProductAt(index);
    showProduct(p);

    int qty = readInt("Quantity to add : ", 1, MAX_QTY);
    if (p.getQuantity() + qty > MAX_QTY) {
        cout << "Error: total stock cannot go above " << MAX_QTY << ".\n";
        return;
    }

    p.setQuantity(p.getQuantity() + qty);
    pm.updateProduct(p);
    tl.append(Transaction(tl.getNextID(), id, "IN", qty));
    saveAll();
    cout << " Stock added. New quantity: " << p.getQuantity() << "\n";
}

void stockOut() {
    cout << "\n--- Stock Out ---\n";
    if (pm.isEmpty()) {
        cout << "No products in inventory.\n";
        return;
    }

    int id = readInt("Enter Product ID: ", 1, MAX_ID);
    int index = pm.searchById(id);
    if (index == -1) {
        cout << "Error: product not found.\n";
        return;
    }

    Product p = pm.getProductAt(index);
    showProduct(p);

    if (p.getQuantity() == 0) {
        cout << "Error: this product has zero stock. Stock-out is not possible.\n";
        return;
    }

    int qty = readInt("Quantity to remove: ", 1, MAX_QTY);
    if (qty > p.getQuantity()) {
        cout << "Error: insufficient stock. Available: " << p.getQuantity()
             << ", requested: " << qty << ".\n";
        return;
    }

    p.setQuantity(p.getQuantity() - qty);
    pm.updateProduct(p);
    tl.append(Transaction(tl.getNextID(), id, "OUT", qty));
    saveAll();
    cout << "Stock removed. New quantity: " << p.getQuantity() << "\n";

    if (p.isLowStock()) {
        cout << " WARNING: '" << p.getProductName() << "' is now LOW on stock (threshold "
             << p.getThreshold() << ").\n";
    }
}

void inventoryReport() {
    cout << "\n--- Inventory Report ---\n";
    if (pm.isEmpty()) {
        cout << "No products in inventory.\n";
        return;
    }

    pm.displayAll();

    int total = 0, low = 0, zero = 0;
    long long units = 0;
    double value = 0;

    // getProductAt() returns an empty product after the last one
    for (int i = 0; pm.getProductAt(i).getProductID() != 0; i++) {
        Product p = pm.getProductAt(i);
        total++;
        units += p.getQuantity();
        value += p.getQuantity() * p.getPrice();
        if (p.isLowStock()) low++;
        if (p.getQuantity() == 0) zero++;
    }

    cout << "\nTotal products      : " << total
         << "\nTotal units in stock: " << units
         << "\nTotal stock value   : " << fixed << setprecision(2) << value
         << "\nLow-stock products  : " << low
         << "\nOut-of-stock items  : " << zero << "\n";
}

void showMenu() {
    cout << "\n=========================================\n"
         << "   INVENTORY & STOCK MANAGEMENT SYSTEM\n"
         << "=========================================\n"
         << " 1. Add Product\n"
         << " 2. Update Product\n"
         << " 3. Delete Product\n"
         << " 4. Search Product\n"
         << " 5. Sort Inventory\n"
         << " 6. Display All Products\n"
         << " 7. Stock In\n"
         << " 8. Stock Out\n"
         << " 9. View Transaction History\n"
         << "10. Low-Stock Report\n"
         << "11. Full Inventory Report\n"
         << " 0. Save and Exit\n"
         << "-----------------------------------------\n";
}

int main() {
    pm.loadFromFile();
    tl.loadFromFile();

    while (true) {
        showMenu();
        int choice = readInt("Enter your choice (0-11): ", 0, 11);

        if (choice == 1) addProduct();
        else if (choice == 2) updateProduct();
        else if (choice == 3) deleteProduct();
        else if (choice == 4) searchProduct();
        else if (choice == 5) sortProducts();
        else if (choice == 6) {
            cout << "\n--- All Products ---\n";
            pm.displayAll();
        }
        else if (choice == 7) stockIn();
        else if (choice == 8) stockOut();
        else if (choice == 9) {
            cout << "\n--- Transaction History ---\n";
            tl.display();
        }
        else if (choice == 10) {
            cout << "\n--- Low-Stock Report ---\n";
            pm.displayLowStock();
        }
        else if (choice == 11) inventoryReport();
        else {
            saveAll();
            cout << "Data saved. Goodbye!\n";
            return 0;
        }
    }
}