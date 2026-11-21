#ifndef TRANSACTION_H
#define TRANSACTION_H
#include <iostream>
#include <string>
using namespace std;

class Transaction{
    public:
    int transactionID;
    int productID;
    string type;
    int quantity;

    Transaction(){
        transactionID = 0;
        productID = 0;
        type = "";
        quantity = 0;
    }
    Transaction(int tid, int pid, string t, int q){
        transactionID = tid;
        productID = pid;
        type = t;
        quantity = q;
    }

    void display(){
        cout<<transactionID<<"\t"<<productID<<"\t"<<type<<"\t"<<quantity<<"\t";
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