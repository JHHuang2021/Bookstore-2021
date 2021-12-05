#include <bits/stdc++.h>

using namespace std;
ifstream in;
ofstream out;

class lib {
private:
    char isbn[20];
    char book_name[60], author[60], keyword[60], quantity[10];
    double price, total_cost;

public:
    lib();

    ~lib();

    void show(const char *);

    void buy(const char *);

    void select(const char *);

    void modify(const char *);

    void import(const char *);
};

void find_book(const lib &);

void insert_book(const lib &);

void modify_book(const lib &);

void delete_book(const lib &);