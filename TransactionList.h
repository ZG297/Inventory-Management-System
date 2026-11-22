#ifndef TRANSACTION_LIST_H
#define TRANSACTION_LIST_H

#include "Transaction.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
using namespace std;

// Singly linked list of transactions
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

    // Deletes all nodes and resets the list
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

    // Adds a transaction at the end of the list
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

    // Prints all transactions in a table
    void display() const {
        if (head == NULL) {
            cout << "No transactions yet." << endl;
            return;
        }

        cout << "ID\tProduct\tType\tQty\tDate" << endl;
        cout << "-------------------------------------------------" << endl;

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
            file << current->data.transactionID << "|"
                 << current->data.productID << "|"
                 << current->data.type << "|"
                 << current->data.quantity << "|"
                 << current->data.date << endl;
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

        string sid, spid, type, sqty, date;
        while (getline(file, sid, '|')) {
            getline(file, spid, '|');
            getline(file, type, '|');
            getline(file, sqty, '|');
            getline(file, date);

            // atoi converts text to a number
            int tid = atoi(sid.c_str());
            int pid = atoi(spid.c_str());
            int qty = atoi(sqty.c_str());

            append(Transaction(tid, pid, type, qty, date));
        }
        file.close();
    }
};

#endif