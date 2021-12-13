#include "lib.h"

#include <cstdlib>
#include <cstring>
#include <string>

#include "error.h"
#include "filemap.hpp"
#include "parser.h"

Book::Book() {}

Book::Book(string &ISBN, string &book_name, string &author, string &keyword) {
    strcpy(this->ISBN_, ISBN.c_str());
    strcpy(this->book_name_, book_name.c_str());
    strcpy(this->author_, author.c_str());
    strcpy(this->keyword_, keyword.c_str());
}

Book::~Book() {}

const string Book::GetISBN() const { return string(this->ISBN_); }
const string Book::GetBookName() const { return string(this->book_name_); }
const string Book::GetAuthor() const { return string(this->author_); }
const string Book::GetKeyword() const { return string(this->keyword_); };
const int Book::GetQuantity() const { return this->quantity_; }
const double Book::GetPrice() const { return this->price_; }

bool Book::operator<(Book &obj) { return this->GetISBN() < obj.GetISBN(); }

bool Book::operator=(Book &obj) const {
    if (strcmp(this->book_name_, "") != 0 &&
        strcmp(this->book_name_, obj.book_name_) != 0)
        return false;
    if (strcmp(this->ISBN_, "") != 0 && strcmp(this->ISBN_, obj.ISBN_) != 0)
        return false;
    if (strcmp(this->author_, "") != 0 &&
        strcmp(this->author_, obj.author_) != 0)
        return false;
    if (strcmp(this->keyword_, "") != 0 &&
        strstr(obj.keyword_, this->keyword_) != nullptr)
        return false;
    return true;
}

void Show(TokenScanner &line) {
    MainInfo<Book> book_info("book_info");
    set<Book> find;
    char tmp[61];
    string ISBN, book_name, author, keyword;
    strcpy(tmp, line.nextToken().c_str());
    char *token = strtok(tmp, "=");
    if (string(token) == "-ISBN") ISBN = string(strtok(nullptr, " "));
    if (string(token) == "-name") book_name = string(strtok(nullptr, " "));
    if (string(token) == "-author") author = string(strtok(nullptr, " "));
    if (string(token) == "-keyword") keyword = string(strtok(nullptr, " "));
    book_info.FindInfo(Book(ISBN, book_name, author, keyword), find);
    while (!find.empty()) {
        // [ISBN]\t[Book-Name]\t[Author]\t[Keyword]\t[Price]\t[¿â´æÊýÁ¿]\n
        auto iter = find.begin();
        cout << iter->GetISBN() << "\t" << iter->GetBookName() << "\t"
             << iter->GetAuthor() << "\t" << iter->GetKeyword() << "\t"
             << iter->GetPrice() << "\t" << iter->GetQuantity() << "\n";
        find.erase(iter);
    }
}

void BuyBook(TokenScanner &line) {
    MainInfo<Book> book_info("book_info");
    string ISBN, quantity;
    ISBN = line.nextToken(), quantity = line.nextToken();
    Book tmp = book_info.FindInfo(ISBN);
    if(tmp.GetQuantity()<atoi(quantity.c_str())) throw Error();
    //todo
}

void FindBook(const Book &) {}

void InsertBook(const Book &) {}

void ModifyBook(const Book &) {}

void DeleteBook(const Book &) {}
