#ifndef PRODUCT_MANAGER_H
#define PRODUCT_MANAGER_H

#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <cctype>
#include "Product.h"
using namespace std;

class ProductManager {
	private:
		Product* products;
		int size;
		int capacity;
		
		void resize () {
			// double the array's max capacity
			capacity *= 2;
			Product* tempProducts = new Product[capacity];
			
			for (int i = 0; i < size; i++) {
				tempProducts[i] = products[i];
			}
			
			delete[] products;
			products = tempProducts;
		}
		
	public:
		ProductManager () {
			capacity = 50;
			size = 0;
			products = new Product[capacity];
		}
		
		~ProductManager () {
			delete[] products;
		}
		
		
		// ===== add / delete products =====
		bool addProduct (const Product& p) {
			if (isExistingId(p.getProductID())) return false;
			
			if (isInvalid(p)) return false;
			
			if (size == capacity) {
				resize();
			}
			
			products[size++] = p;
			return true;
		}
		
		bool deleteProduct (int id) {
			int pIndex = -1;
			for (int i = 0; i < size; i++) {
				if (products[i].getProductID() == id) {
					pIndex = i;
					break;
				}
			}
			
			// product Id not found
			if (pIndex == -1) return false;
			
			// shift elements to the left
			for (int i = pIndex; i < size - 1; i++) {
				products[i] = products[i + 1];
			}
			
			size--;
			return true;
		}
		
		bool updateProduct (const Product& p) {
			int pIndex = searchById(p.getProductID());
			
			if (pIndex == -1) return false;
			if (isInvalid(p)) return false;
			
			products[pIndex] = p;
			return true;
		}
		
		
		// ===== helper functions =====
		bool isExistingId (int id) {
			for (int i = 0; i < size; i++) {
				if (products[i].getProductID() == id) return true;
			}
			
			return false;
		}
		
		bool isInvalid (const Product& p) {
			return (p.getProductID() <= 0 || 
				    p.getProductName().empty() || 
					p.getQuantity() < 0 || 
					p.getPrice() <= 0 || 
					p.getThreshold() < 0);
		}
		
		bool isEmpty () {
			return size == 0;
		}
		
		string toLower (string s) {
			for (int i = 0; i < s.length(); i++) {
				s[i] = tolower(s[i]);
			}
			
			return s;
		}
		
		Product getProductAt (int index) {
			if (index < 0 || index >= size) return Product();
			
			return products[index];
		}
		
		
		// ===== search functions =====
		int searchById (int id) {
			for (int i = 0; i < size; i++) {
				if (products[i].getProductID() == id) 
					return i;
			}
			
			// product not found
			return -1;
		}
		
		int searchByName (string n) {
			n = toLower(n);
			
			for (int i = 0; i < size; i++) {
				if (toLower(products[i].getProductName()) == n) 
					return i;
			}
			
			// product not found
			return -1;
		}
		
		// searching for multiple matching results
		void searchByNamePrefix (string prefix) {
			prefix = toLower(prefix);
			bool found = false;
			
			for (int i = 0; i < size; i++) {
				if (toLower(products[i].getProductName()).find(prefix) == 0) {
					if (!found) {
						Product::printHeader();
						found = true;
					}
					
					products[i].display();
				}
			}
			
			// nothing printed
			if (!found) cout << "No product found starting with '" << prefix << "'. \n";
		}
		
		
		// ===== sorting functions =====
		void sortById () {
			// selection sort
			for (int i = 0; i < size - 1; i++) {
				int minIndex = i;
				
				for (int j = i + 1; j < size; j++) {
					if (products[j].getProductID() < products[minIndex].getProductID()) {
						minIndex = j;
					}
				}
				
				// swap current with min
				Product temp = products[i];
				products[i] = products[minIndex];
				products[minIndex] = temp;
			}
		}
		
		void sortByName () {
			// selection sort
			for (int i = 0; i < size - 1; i++) {
				int minIndex = i;
				
				for (int j = i + 1; j < size; j++) {
					if (toLower(products[j].getProductName()) < toLower(products[minIndex].getProductName())) {
						minIndex = j;
					}
				}
				
				// swap current with min
				Product temp = products[i];
				products[i] = products[minIndex];
				products[minIndex] = temp;
			}
		}
		
		void sortByQuantity () {
			// selection sort
			for (int i = 0; i < size - 1; i++) {
				int minIndex = i;
				
				for (int j = i + 1; j < size; j++) {
					if (products[j].getQuantity() < products[minIndex].getQuantity()) {
						minIndex = j;
					}
				}
				
				// swap current with min
				Product temp = products[i];
				products[i] = products[minIndex];
				products[minIndex] = temp;
			}
		}
		
		
		// ===== display functions =====
		void displayAll () {
			if (isEmpty()) {
				cout << "No products in inventory. \n";
				return;
			}
			
			Product::printHeader();
			for (int i = 0; i < size; i++) {
				products[i].display();
			}
		}
		
		void displayLowStock () {
			if (isEmpty()) {
				cout << "No products in inventory. \n";
				return;
			}
			
			bool found = false;
			
			for (int i = 0; i < size; i++) {
				if (products[i].getQuantity() <= products[i].getThreshold()) {
					if (!found) {
						Product::printHeader();
						found = true;
					}
					
					products[i].display();
				}
			}
			
			if (!found) cout << "No items in Low Stock. \n";
		}
		
		
		// ===== file I/O handling =====
		void saveToFile (const string& filename = "products.txt") {
			ofstream out(filename);
			if (!out) {
				cout << "Error: could not open file, " << filename << ", for saving. \n";
				return;
			}
			
			out << fixed << setprecision(2);
			
			for (int i = 0; i < size; i++) {
				out << products[i].getProductID() << "|"
				    << products[i].getProductName() << "|"
				    << products[i].getQuantity() << "|"
				    << products[i].getPrice() << "|"
				    << products[i].getThreshold() << "\n";
			}
		}
		
		void loadFromFile (const string& filename = "products.txt") {
			ifstream in(filename);
			if (!in) return;  // no file exists yet
			
			string line;
			int skipped = 0;
			
			while (getline(in, line)) {  // read one line at a time
				if (line.empty()) continue;
				
				stringstream ss(line);
				string strId, name, strQty, strPrice, strThreshold;
				
				getline(ss, strId, '|');
				getline(ss, name, '|');
				getline(ss, strQty, '|');
				getline(ss, strPrice, '|');
				getline(ss, strThreshold, '|');
				
				try {
					// add line read to product list array
					Product p(stoi(strId), name, stoi(strQty), stod(strPrice), stoi(strThreshold));
					
					if (!addProduct(p)) skipped++;
					
				} catch (...) {
					// skip correupted lines
					skipped++;
				}
			}
			
			if (skipped > 0) {
				cout << skipped << " invalid line(s) skipped while loading. \n";
			}
		}
};

#endif