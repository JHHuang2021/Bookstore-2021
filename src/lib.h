#ifndef LIB_H
#define LIB_H

#include <bits/stdc++.h>

using namespace std;

class Book {
   private:
    char ISBN[20];
    char Book_Name[60];
    char Author[60];
    char Keyword[60];
    int Quantity = 0;  //库存数量
    double price = 0;  //单价
   public:
    Book();
    ~Book();
    void show(const char *);
    void modify(const char *);
    void import(const char *);
};

void find_book(const Book &);
void insert_book(const Book &);
void modify_book(const Book &);
void delete_book(const Book &);

#endif