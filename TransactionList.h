#ifndef TRANSACTION_LIST_H
#define TRANSACTION_LIST_H

#include "Transaction.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class TransactionList {
private:
    TransactionNode* head;
    TransactionNode* tail;
    int size;

public:
    TransactionList() {
        head = NULL;
        tail = NULL;
        size = 0;
    }

    ~TransactionList() {
        clear();
    }

    void clear() {
        TransactionNode* current = head;
        while (current != NULL) {
            TransactionNode* nextNode = current->next;
            delete current;
            current = nextNode;
        }
        head = NULL;
        tail = NULL;
        size = 0;
    }

    void append(Transaction t) {
        TransactionNode* newNode = new TransactionNode(t);

        if (head == NULL) {
            head = newNode;
            tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
        size++;
    }

    void display() const {
        if (head == NULL) {
            cout << "No transactions yet." << endl;
            return;
        }

        cout << "ID\tProduct\tType\tQty" << endl;
        cout << "-----------------------------" << endl;

        TransactionNode* current = head;
        while (current != NULL) {
            current->data.display();
            current = current->next;
        }
    }

    int count() const {
        return size;
    }

    int getNextID() const {
        if (tail == NULL) return 1;
        return tail->data.transactionID + 1;
    }

    void saveToFile(const string& filename = "transactions.txt") const {
        ofstream file(filename.c_str());
        if (!file) {
            cout << "Error: could not open " << filename << " for saving." << endl;
            return;
        }

        TransactionNode* current = head;
        while (current != NULL) {
            file << "ID: " << current->data.transactionID
                 << " Product: " << current->data.productID
                 << " Type: " << current->data.type
                 << " Qty: " << current->data.quantity << endl;
            current = current->next;
        }
        file.close();
    }

    void loadFromFile(const string& filename = "transactions.txt") {
        ifstream file(filename.c_str());
        if (!file) {
            return;
        }

        clear();

        int tid, pid, qty;
        string type, l1, l2, l3, l4;
        while (file >> l1 >> tid >> l2 >> pid >> l3 >> type >> l4 >> qty) {
            append(Transaction(tid, pid, type, qty));
        }
        file.close();
    }
};

#endif