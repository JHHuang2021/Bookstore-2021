#ifndef LIB_H
#define LIB_H

#include <bits/stdc++.h>

#include "parser.h"

using namespace std;

class Book {
    friend Book Select(TokenScanner &line);
    friend void ModifyBook(Book &book, TokenScanner &line);
    friend void BuyBook(TokenScanner &line);
    friend void Import(Book &book, int quantity, double total_cost);

   private:
    int index_ = 0;
    char ISBN_[21];
    char book_name_[61];
    char author_[61];
    char keyword_[61];
    int quantity_ = 0;  //库存数量
    double price_ = 0;  //单价
   public:
    bool operator==(const Book &obj) const;
    bool operator<(const Book &obj) const;
    Book &operator=(const Book obj);
    void operator()(const Book &obj);
    Book();
    Book(string ISBN, string book_name, string author, string keyword,
         int index, int quantity = 0, double price = 0);
    ~Book();
    const string GetISBN() const;
    const string GetBookName() const;
    const string GetAuthor() const;
    const string GetKeyword() const;
    const int GetQuantity() const;
    const double GetPrice() const;
};

void Show(TokenScanner &line, int index);
void BuyBook(TokenScanner &line);
Book Select(TokenScanner &line);
Book Select(const string &ISBN);
void ModifyBook(Book &book, TokenScanner &line);
void Import(Book &book, int quantity, double total_cost);

#endif