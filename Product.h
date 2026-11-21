#ifndef PRODUCT_H
#define PRODUCT_H

#include <iostream>
#include <string>
#include <iomanip>
using namespace std;

class Product {
	private:
		int productID;
		string name;
		int quantity;
		double price;
		int threshold;
	
	public:
		Product () {
			productID = 0;
			name = "";
			quantity = 0;
			price = 0.0;
			threshold = 0;
		}
		
		Product (int id, string n, int q, double p, int t) {
			productID = id;
			name = n;
			quantity = q;
			price = p;
			threshold = t;
		}
		
		
		// getters
		int getProductID () const {
			return productID;
		}
		
		string getProductName () const {
			return name;
		}
		
		int getQuantity () const {
			return quantity;
		}
		
		double getPrice () const {
			return price;
		}
		
		int getThreshold () const {
			return threshold;
		}
		
		
		// setters
		void setProductName (string n) {
			name = n;
		}
		
		void setQuantity (int q) {
			quantity = q;
		}
		
		void setPrice (double p) {
			price = p;
		}
		
		void setThreshold (int t) {
			threshold = t;
		}
		
		
		// helper functions
		static void printHeader () {
			cout << left << setw(12) << "Product ID" << setw(20) << "Name" 
			     << setw(10) << "Quantity" << setw(12) << "Price" 
			     << setw(10) << "Threshold" << "\n"
			     << string(64, '-') << "\n";
		}
		
		void display () {
			cout << left << setw(12) << productID << setw(20) << name
			     << setw(10) << quantity << setw(12) << fixed << setprecision(2) << price
			     << setw(10) << threshold << "\n";
		}
		
		bool isLowStock () {
			return quantity <= threshold;
		}
};

#endif