#ifndef LIB_H
#define LIB_H

#include <bits/stdc++.h>

#include "parser.h"

using namespace std;

class Book {
   private:
    char ISBN_[21];
    char book_name_[61];
    char author_[61];
    char keyword_[61];
    int quantity_ = 0;  //库存数量
    double price_ = 0;  //单价
   public:
    bool operator=(Book &obj) const;
    bool operator<(Book &obj);
    Book();
    Book(string &ISBN, string &book_name, string &author, string &keyword);
    ~Book();
    const string GetISBN() const;
    const string GetBookName() const;
    const string GetAuthor() const;
    const string GetKeyword() const;
    const int GetQuantity() const;
    const double GetPrice() const;
};

void Show(TokenScanner &line);
void BuyBook(TokenScanner &line);
void InsertBook(const Book &);
void ModifyBook(const Book &);

#endif