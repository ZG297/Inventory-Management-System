#ifndef TRANSACTION_H
#define TRANSACTION_H
#include <iostream>
#include <string>
#include <ctime>
using namespace std;

// Returns the current date and time as text
string getCurrentDate(){
    time_t now = time(0);
    tm* t = localtime(&now);
    char buffer[20];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", t);
    return string(buffer);
}

class Transaction{
    public:
    int transactionID;
    int productID;
    string type;        // "IN" or "OUT"
    int quantity;
    string date;        

    Transaction(){
        transactionID = 0;
        productID = 0;
        type = "";
        quantity = 0;
        date = "";
    }

    // For new transactions, date is set automatically
    Transaction(int tid, int pid, string t, int q){
        transactionID = tid;
        productID = pid;
        type = t;
        quantity = q;
        date = getCurrentDate();
    }

    Transaction(int tid, int pid, string t, int q, string d){
        transactionID = tid;
        productID = pid;
        type = t;
        quantity = q;
        date = d;
    }

    void display(){
        cout<<transactionID<<"\t"<<productID<<"\t"<<type<<"\t"<<quantity<<"\t"<<date;
        cout<<endl;
    }
};

class TransactionNode {
public:
    Transaction data;
    TransactionNode* next;
    TransactionNode(Transaction t){
        data = t;
        next = NULL;
    } 
};
#endif