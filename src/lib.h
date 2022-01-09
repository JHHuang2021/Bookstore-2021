#ifndef LIB_H
#define LIB_H

#include <bits/stdc++.h>

#include "account.h"
#include "parser.h"

class Book {
    friend Book Select(Account &account, TokenScanner &line);
    friend void ModifyBook(Account &account, Book &book, TokenScanner &line);
    friend void BuyBook(Account &account, TokenScanner &line);
    friend void Import(Account &account, Book &book, int quantity,
                       double total_cost);

   private:
    int index_ = 0;
    char ISBN_[21];
    char book_name_[61];
    char author_[61];
    char keyword_[61];
    long long quantity_ = 0;  //库存数量
    double price_ = 0;        //单价
   public:
    bool operator==(const Book &obj) const;
    bool operator<(const Book &obj) const;
    Book &operator=(const Book obj);
    void operator()(const Book &obj);
    Book();
    Book(std::string ISBN, std::string book_name, std::string author, std::string keyword,
         int index, int quantity = 0, double price = 0);
    ~Book();
    const std::string GetISBN() const;
    const std::string GetBookName() const;
    const std::string GetAuthor() const;
    const std::string GetKeyword() const;
    const int GetIndex() const;
    const int GetQuantity() const;
    const double GetPrice() const;
};

std::stringstream Show(Account &account, TokenScanner &line, int index);
void BuyBook(Account &account, TokenScanner &line);
Book Select(Account &account, TokenScanner &line);
Book Select(const int index);
void ModifyBook(Account &account, Book &book, TokenScanner &line);
void Import(Account &account, Book &book, int quantity, double total_cost);

#endif