#include "lib.h"

#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <ios>
#include <string>

#include "error.h"
#include "filemap.hpp"
#include "log.h"
#include "parser.h"

Book::Book() {}

Book::Book(string ISBN, string book_name, string author, string keyword,
           int index, int quantity, double price) {
    strcpy(this->ISBN_, ISBN.c_str());
    strcpy(this->book_name_, book_name.c_str());
    strcpy(this->author_, author.c_str());
    strcpy(this->keyword_, keyword.c_str());
    this->index_ = index;
    this->quantity_ = quantity;
    this->price_ = price;
}

Book::~Book() {}

const string Book::GetISBN() const { return string(this->ISBN_); }
const string Book::GetBookName() const { return string(this->book_name_); }
const string Book::GetAuthor() const { return string(this->author_); }
const string Book::GetKeyword() const { return string(this->keyword_); };
const int Book::GetQuantity() const { return this->quantity_; }
const double Book::GetPrice() const { return this->price_; }

Book &Book::operator=(const Book obj) {
    strcpy(this->book_name_, obj.book_name_);
    strcpy(this->author_, obj.author_);
    strcpy(this->ISBN_, obj.ISBN_);
    strcpy(this->keyword_, obj.keyword_);
    this->index_ = obj.index_;
    this->price_ = obj.price_;
    this->quantity_ = obj.quantity_;
    return *this;
}

bool Book::operator<(const Book &obj) const {
    return this->GetISBN() < obj.GetISBN();
}

bool Book::operator==(const Book &obj) const {
    if (strcmp(this->book_name_, "") != 0 &&
        strcmp(this->book_name_, obj.book_name_) != 0)
        return false;
    if (strcmp(this->ISBN_, "") != 0 && strcmp(this->ISBN_, obj.ISBN_) != 0)
        return false;
    if (strcmp(this->author_, "") != 0 &&
        strcmp(this->author_, obj.author_) != 0)
        return false;
    if (strcmp(this->keyword_, "") != 0) {
        char tmp[61];
        strcpy(tmp, obj.keyword_);
        char *token = strtok(tmp, "|");
        while (token != nullptr) {
            if (strcmp(this->keyword_, token) == 0) return true;
            token = strtok(nullptr, "|");
        }
        return false;
    }
    return true;
}

void Show(TokenScanner &line) {
    MainInfo<Book> book_info("book_info");
    set<Book> find;
    char tmp[61];
    string ISBN = "", book_name = "", author = "", keyword = "";
    strcpy(tmp, line.nextToken().c_str());
    char *token = strtok(tmp, "=");
    if (string(token) == "finance") {
        int times = atoi(line.nextToken().c_str());
        Log log("log");
        log.ShowFinance(times);
        return;
    } else if (string(token) == "-ISBN") {
        ISBN = string(strtok(nullptr, " "));
        if (ISBN == "") throw Error();
    } else if (string(token) == "-name") {
        book_name = string(strtok(nullptr, " "));
        book_name = book_name.substr(1, book_name.length() - 2);  //
        if (book_name == "") throw Error();
    } else if (string(token) == "-author") {
        author = string(strtok(nullptr, " "));
        author = author.substr(1, author.length() - 2);  //
        if (author == "") throw Error();
    } else if (string(token) == "-keyword") {
        keyword = string(strtok(nullptr, " "));
        keyword = keyword.substr(1, keyword.length() - 2);  //
        if (keyword == "") throw Error();
    }
    book_info.FindInfo(Book(ISBN, book_name, author, keyword, 0), find);
    if (find.empty())
        cout << "\n";
    else
        while (!find.empty()) {
            // [ISBN]\t[Book-Name]\t[Author]\t[Keyword]\t[Price]\t[¿â´æÊýÁ¿]\n
            auto iter = find.begin();
            cout << iter->GetISBN() << "\t" << iter->GetBookName() << "\t"
                 << iter->GetAuthor() << "\t" << iter->GetKeyword() << "\t"
                 << fixed << setprecision(2) << iter->GetPrice() << "\t"
                 << iter->GetQuantity() << "\n";
            find.erase(iter);
        }
}

void BuyBook(TokenScanner &line) {
    MainInfo<Book> book_info("book_info");
    string ISBN, quantity;
    ISBN = line.nextToken(), quantity = line.nextToken();
    Book tmp = book_info.FindInfo(ISBN);
    int quantity_int = atoi(quantity.c_str());
    if (tmp.quantity_ < quantity_int)
        throw Error();
    else
        tmp.quantity_ -= quantity_int;
    // Delete may return T to make the program faster
    book_info.ModifyInfo(tmp, tmp.index_, tmp.ISBN_, tmp.ISBN_);
    cout << fixed << setprecision(2) << quantity_int * tmp.price_ << endl;
    Log log("log");
    log.Record(to_string(quantity_int * tmp.price_));
}

Book Select(TokenScanner &line) {
    MainInfo<Book> book_info("book_info");
    string ISBN = line.nextToken();
    try {
        return book_info.FindInfo(ISBN);
    } catch (Error &ex) {
        Book tmp(ISBN, "", "", "", book_info.GetNum());
        book_info.WriteInfo(tmp, ISBN);
        return tmp;
    }
}

void ModifyBook(Book &book, TokenScanner &line) {
    MainInfo<Book> book_info("book_info");
    char tmp[61];
    string old_ISBN = book.ISBN_;
    string ISBN, book_name, author, keyword, price;
    string nxt_token = line.nextToken();
    while (nxt_token != "-1") {
        strcpy(tmp, nxt_token.c_str());
        char *token = strtok(tmp, "=");
        if (string(token) == "-ISBN") {
            if (ISBN != "") throw Error();
            ISBN = string(strtok(nullptr, " "));
            if (ISBN == "" || strcmp(ISBN.c_str(), book.ISBN_) == 0)
                throw Error();
            book_info.FindInfo(ISBN, 1);//to be modified
            strcpy(book.ISBN_, ISBN.c_str());
        } else if (string(token) == "-name") {
            if (book_name != "") throw Error();
            book_name = string(strtok(nullptr, " "));
            book_name = book_name.substr(1, book_name.length() - 2);  //
            if (book_name == "") throw Error();
            strcpy(book.book_name_, book_name.c_str());
        } else if (string(token) == "-author") {
            if (author != "") throw Error();
            author = string(strtok(nullptr, " "));
            author = author.substr(1, author.length() - 2);  //
            if (author == "") throw Error();
            strcpy(book.author_, author.c_str());
        } else if (string(token) == "-keyword") {
            if (keyword != "") throw Error();
            keyword = string(strtok(nullptr, " "));
            keyword = keyword.substr(1, keyword.length() - 2);  //
            if (keyword == "") throw Error();
            strcpy(book.keyword_, keyword.c_str());
        } else if (string(token) == "-price") {
            if (price != "") throw Error();
            price = string(strtok(nullptr, " "));
            if (price == "") throw Error();
            book.price_ = atof(price.c_str());
        }
        nxt_token = line.nextToken();
    }
    book_info.ModifyInfo(book, book.index_, old_ISBN, book.ISBN_);
}

void Import(Book &book, int quantity, double total_cost) {
    MainInfo<Book> book_info("book_info");
    book.quantity_ += quantity;
    book_info.ModifyInfo(book, book.index_, book.ISBN_, book.ISBN_);
}
