#include "log.h"

#include <bits/stdc++.h>

#include <cstdlib>
#include <cstring>
#include <fstream>
#include <future>
#include <iomanip>
#include <sstream>

#include "account.h"
#include "error.h"
using namespace std;
Log::Log(string file_name) {
    this->file_name_ = file_name;
    ifstream in_1(file_name, ifstream::in);

    if (!in_1) {
        ofstream out(file_name, ofstream::out);
        int num = 0;
        fstream ffile(file_name, fstream::in | fstream::binary | fstream::out);
        ffile.write(reinterpret_cast<char *>(&num), sizeof(int));
        ffile.close();
    }
}

Log::~Log() {}

void Log::Record(string rec) {
    int num = 0;
    fstream ffile(file_name_, fstream::in | fstream::binary | fstream::out);
    ffile.seekg(0);
    ffile.read(reinterpret_cast<char *>(&num), sizeof(int));
    num++;
    ffile.seekp(0);
    ffile.write(reinterpret_cast<char *>(&num), sizeof(int));
    char rec_char[15];
    strcpy(rec_char, rec.c_str());
    ffile.seekp(sizeof(int) + (num - 1) * sizeof(rec_char));
    ffile.write(reinterpret_cast<char *>(rec_char), sizeof(rec_char));
    ffile.close();
}

void Log::ShowFinance(int times) {
    if (times == 0) {
        cout << "\n";
        return;
    }
    int num = 0;
    fstream ffile(file_name_, fstream::in | fstream::binary | fstream::out);
    ffile.seekg(0);
    ffile.read(reinterpret_cast<char *>(&num), sizeof(int));
    // if (num == 0) {
    //     cout << "+ 0.00 - 0.00\n";
    //     ffile.close();
    //     return;
    // }
    if (times > num)
        throw Error();
    else if (times == -1)
        times = num;
    double in = 0, out = 0;
    for (int i = num - 1; i >= num - times; i--) {
        char rec_char[15];
        ffile.seekg(sizeof(int) + i * sizeof(rec_char));
        ffile.read(reinterpret_cast<char *>(rec_char), sizeof(rec_char));
        double bill = atof(rec_char);
        if (bill < 0)
            out += bill;
        else
            in += bill;
    }
    ffile.close();
    if (out < 0)
        cout << "+ " << fixed << setprecision(2) << in << " "
             << "- " << fixed << setprecision(2) << (-out) << "\n";
    else
        cout << "+ " << fixed << setprecision(2) << in << " "
             << "- " << fixed << setprecision(2) << (out) << "\n";
}

LogForAll::LogForAll() {
    const string file_name = "allog";
    ifstream in_1(file_name, ifstream::in);

    if (!in_1) {
        ofstream out(file_name, ofstream::out);
        int num = 0;
        fstream ffile(file_name, fstream::in | fstream::binary | fstream::out);
        ffile.write(reinterpret_cast<char *>(&num), sizeof(int));
        ffile.close();
    }
}

void LogForAll::Record(string rec) {
    int num = 0;
    fstream ffile(file_name_, fstream::in | fstream::binary | fstream::out);
    ffile.seekg(0);
    ffile.read(reinterpret_cast<char *>(&num), sizeof(int));
    num++;
    ffile.seekp(0);
    ffile.write(reinterpret_cast<char *>(&num), sizeof(int));
    char rec_char[100];
    strcpy(rec_char, rec.c_str());
    ffile.seekp(sizeof(int) + (num - 1) * sizeof(rec_char));
    ffile.write(reinterpret_cast<char *>(rec_char), sizeof(rec_char));
    ffile.close();
}

void LogForAll::Show() {
    int num = 0;
    fstream ffile(file_name_, fstream::in | fstream::binary | fstream::out);
    ffile.seekg(0);
    ffile.read(reinterpret_cast<char *>(&num), sizeof(int));
    char rec_char[100];
    for (int i = 0; i < num; i++) {
        memset(rec_char, 0, sizeof(rec_char));
        ffile.seekg(sizeof(int) + i * sizeof(rec_char));
        ffile.read(reinterpret_cast<char *>(rec_char), sizeof(rec_char));
        cout << rec_char << endl;
    }
    ffile.close();
}

void LogForAll::ShowMyself(Account &account) {
    int num = 0;
    fstream ffile(file_name_, fstream::in | fstream::binary | fstream::out);
    ffile.seekg(0);
    ffile.read(reinterpret_cast<char *>(&num), sizeof(int));
    char rec_char[100];
    int pos;
    for (pos = num - 1; pos >= 0; pos--) {
        memset(rec_char, 0, sizeof(rec_char));
        ffile.seekg(sizeof(int) + pos * sizeof(rec_char));
        ffile.read(reinterpret_cast<char *>(rec_char), sizeof(rec_char));
        stringstream ss(rec_char);
        string user_id, action;
        for (int i = 1; i <= 3; i++) {
            ss >> action;
        }
        if (action == "login") {
            ss >> user_id;
            if (account.GetUserId() == user_id)
                break;
            else
                continue;
        }
    }
    for (int i = pos; i < num; i++) {
        memset(rec_char, 0, sizeof(rec_char));
        ffile.seekg(sizeof(int) + i * sizeof(rec_char));
        ffile.read(reinterpret_cast<char *>(rec_char), sizeof(rec_char));
        cout << rec_char << endl;
    }
    ffile.close();
}

void LogForAll::ShowEmployee() {
    int num = 0;
    fstream ffile(file_name_, fstream::in | fstream::binary | fstream::out);
    ffile.seekg(0);
    ffile.read(reinterpret_cast<char *>(&num), sizeof(int));
    char rec_char[100];
    for (int i = 0; i < num; i++) {
        memset(rec_char, 0, sizeof(rec_char));
        ffile.seekg(sizeof(int) + i * sizeof(rec_char));
        ffile.read(reinterpret_cast<char *>(rec_char), sizeof(rec_char));
        stringstream ss(rec_char);
        string priority;
        for (int i = 1; i <= 2; i++) ss >> priority;
        if (priority == "3") cout << rec_char << endl;
    }
    ffile.close();
}

void LogForAll::ShowTransaction() {
    int num = 0;
    fstream ffile(file_name_, fstream::in | fstream::binary | fstream::out);
    ffile.seekg(0);
    ffile.read(reinterpret_cast<char *>(&num), sizeof(int));
    char rec_char[100];
    for (int i = 0; i < num; i++) {
        memset(rec_char, 0, sizeof(rec_char));
        ffile.seekg(sizeof(int) + i * sizeof(rec_char));
        ffile.read(reinterpret_cast<char *>(rec_char), sizeof(rec_char));
        stringstream ss(rec_char);
        string action;
        for (int i = 1; i <= 3; i++) ss >> action;
        if (action == "buy" || action == "import") cout << rec_char << endl;
    }
    ffile.close();
}

LogForAll::~LogForAll() {}