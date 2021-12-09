#ifndef LIB_H
#define LIB_H

#include <bits/stdc++.h>

using namespace std;
ifstream in;
ofstream out;

class book {
   private:
    char ISBN[20];
    char Book_Name[60];
    char Author[60];
    char Keyword[60];
    int Quantity = 0;  //库存数量
    double price = 0;  //单价
   public:
    book();
    ~book();
    void show(const char *);
    void modify(const char *);
    void import(const char *);
};

void find_book(const book &);
void insert_book(const book &);
void modify_book(const book &);
void delete_book(const book &);

#endif