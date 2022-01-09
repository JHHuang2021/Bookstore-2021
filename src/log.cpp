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
Log::Log(std::string file_name) {
    this->file_name_ = file_name;
    std::ifstream in_1(file_name, std::ifstream::in);

    if (!in_1) {
        std::ofstream out(file_name, std::ofstream::out);
        int num = 0;
        std::fstream ffile(file_name, std::fstream::in | std::fstream::binary | std::fstream::out);
        ffile.write(reinterpret_cast<char *>(&num), sizeof(int));
        ffile.close();
    }
}

Log::~Log() {}

void Log::Record(std::string rec) {
    int num = 0;
    std::fstream ffile(file_name_, std::fstream::in | std::fstream::binary | std::fstream::out);
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
        std::cout << "\n";
        return;
    }
    int num = 0;
    std::fstream ffile(file_name_, std::fstream::in | std::fstream::binary | std::fstream::out);
    ffile.seekg(0);
    ffile.read(reinterpret_cast<char *>(&num), sizeof(int));
    // if (num == 0) {
    //     std::cout << "+ 0.00 - 0.00\n";
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
        std::cout << "+ " << std::fixed << std::setprecision(2) << in << " "
             << "- " << std::fixed << std::setprecision(2) << (-out) << "\n";
    else
        std::cout << "+ " << std::fixed << std::setprecision(2) << in << " "
             << "- " << std::fixed << std::setprecision(2) << (out) << "\n";
}

LogForAll::LogForAll() {
    const std::string file_name = "allog";
    std::ifstream in_1(file_name, std::ifstream::in);

    if (!in_1) {
        std::ofstream out(file_name, std::ofstream::out);
        int num = 0;
        std::fstream ffile(file_name, std::fstream::in | std::fstream::binary | std::fstream::out);
        ffile.write(reinterpret_cast<char *>(&num), sizeof(int));
        ffile.close();
    }
}

void LogForAll::Record(std::string rec) {
    int num = 0;
    std::fstream ffile(file_name_, std::fstream::in | std::fstream::binary | std::fstream::out);
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
    std::fstream ffile(file_name_, std::fstream::in | std::fstream::binary | std::fstream::out);
    ffile.seekg(0);
    ffile.read(reinterpret_cast<char *>(&num), sizeof(int));
    char rec_char[100];
    for (int i = 0; i < num; i++) {
        memset(rec_char, 0, sizeof(rec_char));
        ffile.seekg(sizeof(int) + i * sizeof(rec_char));
        ffile.read(reinterpret_cast<char *>(rec_char), sizeof(rec_char));
        std::cout << rec_char << std::endl;
    }
    ffile.close();
}

void LogForAll::ShowMyself(Account &account) {
    int num = 0;
    std::fstream ffile(file_name_, std::fstream::in | std::fstream::binary | std::fstream::out);
    ffile.seekg(0);
    ffile.read(reinterpret_cast<char *>(&num), sizeof(int));
    char rec_char[100];
    int pos;
    for (pos = num - 1; pos >= 0; pos--) {
        memset(rec_char, 0, sizeof(rec_char));
        ffile.seekg(sizeof(int) + pos * sizeof(rec_char));
        ffile.read(reinterpret_cast<char *>(rec_char), sizeof(rec_char));
        std::stringstream ss(rec_char);
        std::string user_id, action;
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
        std::cout << rec_char << std::endl;
    }
    ffile.close();
}

void LogForAll::ShowEmployee() {
    int num = 0;
    std::fstream ffile(file_name_, std::fstream::in | std::fstream::binary | std::fstream::out);
    ffile.seekg(0);
    ffile.read(reinterpret_cast<char *>(&num), sizeof(int));
    char rec_char[100];
    for (int i = 0; i < num; i++) {
        memset(rec_char, 0, sizeof(rec_char));
        ffile.seekg(sizeof(int) + i * sizeof(rec_char));
        ffile.read(reinterpret_cast<char *>(rec_char), sizeof(rec_char));
        std::stringstream ss(rec_char);
        std::string priority;
        for (int i = 1; i <= 2; i++) ss >> priority;
        if (priority == "3") std::cout << rec_char << std::endl;
    }
    ffile.close();
}

void LogForAll::ShowTransaction() {
    int num = 0;
    std::fstream ffile(file_name_, std::fstream::in | std::fstream::binary | std::fstream::out);
    ffile.seekg(0);
    ffile.read(reinterpret_cast<char *>(&num), sizeof(int));
    char rec_char[100];
    for (int i = 0; i < num; i++) {
        memset(rec_char, 0, sizeof(rec_char));
        ffile.seekg(sizeof(int) + i * sizeof(rec_char));
        ffile.read(reinterpret_cast<char *>(rec_char), sizeof(rec_char));
        std::stringstream ss(rec_char);
        std::string action;
        for (int i = 1; i <= 3; i++) ss >> action;
        if (action == "buy" || action == "import") std::cout << rec_char << std::endl;
    }
    ffile.close();
}

LogForAll::~LogForAll() {}