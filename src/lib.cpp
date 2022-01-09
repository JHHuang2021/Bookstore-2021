#include "lib.h"

#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <ios>
#include <sstream>
#include <string>

#include "account.h"
#include "error.h"
#include "filemap.hpp"
#include "log.h"
#include "parser.h"

const std::string kMod = "*-4980(2jofw0.39ac2s@&";
LogForAll all_log;

Book::Book() {
    strcpy(this->ISBN_, "");
    strcpy(this->book_name_, "");
    strcpy(this->author_, "");
    strcpy(this->keyword_, "");
    this->price_ = 0;
    this->quantity_ = 0;
    this->index_ = 0;
}

Book::Book(std::string ISBN, std::string book_name, std::string author, std::string keyword,
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

const std::string Book::GetISBN() const { return std::string(this->ISBN_); }
const std::string Book::GetBookName() const { return std::string(this->book_name_); }
const std::string Book::GetAuthor() const { return std::string(this->author_); }
const std::string Book::GetKeyword() const { return std::string(this->keyword_); };
const int Book::GetIndex() const { return this->index_; }
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

void Book::operator()(const Book &obj) {
    if (strcmp(this->book_name_, "") == 0)
        strcpy(this->book_name_, obj.book_name_);
    if (strcmp(this->author_, "") == 0) strcpy(this->author_, obj.author_);
    if (strcmp(this->ISBN_, "") == 0) strcpy(this->ISBN_, obj.ISBN_);
    if (strcmp(this->keyword_, "") == 0) strcpy(this->keyword_, obj.keyword_);
    if (this->price_ == -1) this->price_ = obj.price_;
    if (this->quantity_ == -1) this->quantity_ = obj.quantity_;
}

std::stringstream Show(Account &account, TokenScanner &line, int priority) {
    std::stringstream ss;
    MainInfo<Book> book_info("book_info");
    std::set<Book> find;
    char tmp[61];
    std::string ISBN = "", book_name = "", author = "", keyword = "";
    strcpy(tmp, line.nextToken().c_str());
    char *token = strtok(tmp, "=");
    if (std::string(token) == "finance") {
        if (priority != 7) throw Error();
        std::string tim = line.nextToken().c_str();
        int times;
        if (tim == kMod)
            times = -1;  //
        else if (IfInvaild(tim.c_str(), 3, 10) ||
                 (tim.length() == 10 && tim > "2147483647"))
            throw Error();
        else
            times = atoi(tim.c_str());
        if (line.nextToken() != kMod) throw Error();
        Log log("log");
        log.ShowFinance(times);
        return std::stringstream("");
    } else if (std::string(token) == "-ISBN") {
        if (line.nextToken() != kMod) throw Error();
        token = strtok(nullptr, " ");
        if (token == nullptr) throw Error();
        ISBN = std::string(token);
        if (ISBN == "" || IfInvaild(ISBN.c_str(), 2, 20)) throw Error();
    } else if (std::string(token) == "-name") {
        if (line.nextToken() != kMod) throw Error();
        token = strtok(nullptr, " ");
        if (token == nullptr || *token != '"' ||
            *(token + strlen(token) - 1) != '"')
            throw Error();
        book_name = std::string(token);
        // book_name = std::string(strtok(nullptr, " "));
        book_name = book_name.substr(1, book_name.length() - 2);  //
        if (book_name == "" || IfInvaild(book_name.c_str(), 4, 60))
            throw Error();
    } else if (std::string(token) == "-author") {
        if (line.nextToken() != kMod) throw Error();
        token = strtok(nullptr, " ");
        if (token == nullptr || *token != '"' ||
            *(token + strlen(token) - 1) != '"')
            throw Error();
        author = std::string(token);
        // author = std::string(strtok(nullptr, " "));
        author = author.substr(1, author.length() - 2);  //
        if (author == "" || IfInvaild(author.c_str(), 4, 60)) throw Error();
    } else if (std::string(token) == "-keyword") {
        if (line.nextToken() != kMod) throw Error();
        token = strtok(nullptr, " ");
        if (token == nullptr || *token != '"' ||
            *(token + strlen(token) - 1) != '"')
            throw Error();
        keyword = std::string(token);
        // keyword = std::string(strtok(nullptr, " "));
        keyword = keyword.substr(1, keyword.length() - 2);  //
        if (keyword == "" || IfInvaild(keyword.c_str(), 4, 60)) throw Error();
        if (strstr(keyword.c_str(), "|") != nullptr) throw Error();
    } else if (std::string(token) != kMod)
        throw Error();
    book_info.FindInfo(Book(ISBN, book_name, author, keyword, 0), find);
    if (find.empty())
        ss << "\n";
    else
        while (!find.empty()) {
            // [ISBN]\t[Book-Name]\t[Author]\t[Keyword]\t[Price]\t[]\n
            auto iter = find.begin();
            ss << iter->GetISBN() << "\t" << iter->GetBookName() << "\t"
               << iter->GetAuthor() << "\t" << iter->GetKeyword() << "\t"
               << std::fixed << std::setprecision(2) << iter->GetPrice() << "\t"
               << iter->GetQuantity() << "\n";
            find.erase(iter);
        }
    all_log.Record(account.GetUserId() + " " +
                   std::to_string(account.GetPriority()) + " show ");
    return ss;
}

void BuyBook(Account &account, TokenScanner &line) {
    MainInfo<Book> book_info("book_info");
    std::string ISBN, quantity;
    ISBN = line.nextToken(), quantity = line.nextToken();
    if (line.nextToken() != kMod || ISBN == kMod || quantity == kMod)
        throw Error();
    Book tmp = book_info.FindInfo(ISBN);
    if (IfInvaild(quantity.c_str(), 3, 10)) throw Error();
    if (quantity.length() == 10 && quantity > "2147483647") throw Error();
    int quantity_int = atoi(quantity.c_str());
    if (tmp.quantity_ < quantity_int)
        throw Error();
    else
        tmp.quantity_ -= quantity_int;
    // Delete may return T to make the program faster
    book_info.ModifyInfo(tmp, tmp.index_, tmp.ISBN_, tmp.ISBN_);
    std::cout << std::fixed << std::setprecision(2) << quantity_int * tmp.price_ << std::endl;
    Log log("log");
    log.Record(std::to_string(quantity_int * tmp.price_));
    LogForAll all_log;
    all_log.Record(account.GetUserId() + " " +
                   std::to_string(account.GetPriority()) + " buy " + ISBN + " " +
                   std::to_string(quantity_int));
}

Book Select(Account &account, TokenScanner &line) {
    MainInfo<Book> book_info("book_info");
    std::string ISBN = line.nextToken();
    if (line.nextToken() != kMod || ISBN == kMod) throw Error();
    if (IfInvaild(ISBN.c_str(), 2, 20)) throw Error();
    all_log.Record(account.GetUserId() + " " +
                   std::to_string(account.GetPriority()) + " select " + ISBN);
    try {
        return book_info.FindInfo(ISBN);
    } catch (Error &ex) {
        Book tmp(ISBN, "", "", "", book_info.GetNum());
        book_info.WriteInfo(tmp, ISBN);
        return tmp;
    }
}

Book Select(const int index) {
    MainInfo<Book> book_info("book_info");
    return book_info.GetInfo(index);
}

void ModifyBook(Account &account, Book &book, TokenScanner &line) {
    MainInfo<Book> book_info("book_info");
    char tmp[61];
    std::string old_ISBN = book.ISBN_;
    std::string ISBN = "", book_name = "", author = "", keyword = "", price = "";
    std::string nxt_token = line.nextToken();
    if (nxt_token == kMod) throw Error();
    while (nxt_token != kMod) {
        strcpy(tmp, nxt_token.c_str());
        char *token = strtok(tmp, "=");
        if (std::string(token) == "-ISBN") {
            if (ISBN != "") throw Error();
            token = strtok(nullptr, " ");
            if (token == nullptr) throw Error();
            ISBN = std::string(token);
            if (ISBN == "")
                // if (ISBN == "" || strcmp(ISBN.c_str(), book.ISBN_) == 0)
                throw Error();
            book_info.FindInfo(ISBN, 1);  // to be modified
            if (IfInvaild(ISBN.c_str(), 2, 20)) throw Error();
            strcpy(book.ISBN_, ISBN.c_str());
        } else if (std::string(token) == "-name") {
            if (book_name != "") throw Error();
            token = strtok(nullptr, " ");
            if (token == nullptr || *token != '"' ||
                *(token + strlen(token) - 1) != '"')
                throw Error();
            book_name = std::string(token);
            // book_name = std::string(strtok(nullptr, " "));
            book_name = book_name.substr(1, book_name.length() - 2);  //
            if (book_name == "" || IfInvaild(book_name.c_str(), 4, 60))
                throw Error();
            strcpy(book.book_name_, book_name.c_str());
        } else if (std::string(token) == "-author") {
            if (author != "") throw Error();
            token = strtok(nullptr, " ");
            if (token == nullptr || *token != '"' ||
                *(token + strlen(token) - 1) != '"')
                throw Error();
            author = std::string(token);
            // author = std::string(strtok(nullptr, " "));
            author = author.substr(1, author.length() - 2);  //
            if (author == "" || IfInvaild(author.c_str(), 4, 60)) throw Error();
            strcpy(book.author_, author.c_str());
        } else if (std::string(token) == "-keyword") {
            if (keyword != "") throw Error();
            token = strtok(nullptr, " ");
            if (token == nullptr || *token != '"' ||
                *(token + strlen(token) - 1) != '"')
                throw Error();
            keyword = std::string(token);
            // keyword = std::string(strtok(nullptr, " "));
            keyword = keyword.substr(1, keyword.length() - 2);  //
            if (keyword == "" || IfKeywordRepeated(keyword.c_str()) ||
                IfInvaild(keyword.c_str(), 4, 60))
                // if (keyword == "")
                throw Error();
            strcpy(book.keyword_, keyword.c_str());
        } else if (std::string(token) == "-price") {
            if (price != "") throw Error();
            token = strtok(nullptr, " ");
            if (token == nullptr) throw Error();
            price = std::string(token);
            // price = std::string(strtok(nullptr, " "));
            if (price == "" || IfInvaild(price.c_str(), 5, 13)) throw Error();
            book.price_ = atof(price.c_str());
        } else
            throw Error();
        nxt_token = line.nextToken();
    }
    book_info.ModifyInfo(book, book.index_, old_ISBN, book.ISBN_);
    all_log.Record(account.GetUserId() + " " +
                   std::to_string(account.GetPriority()) + " modify");
}

void Import(Account &account, Book &book, int quantity, double total_cost) {
    MainInfo<Book> book_info("book_info");
    if (quantity == 0) throw Error();
    book.quantity_ += quantity;
    book_info.ModifyInfo(book, book.index_, book.ISBN_, book.ISBN_);
    all_log.Record(account.GetUserId() + " " +
                   std::to_string(account.GetPriority()) + " import " +
                   book.GetISBN() + " " + std::to_string(quantity) + " " +
                   std::to_string(total_cost));
}
