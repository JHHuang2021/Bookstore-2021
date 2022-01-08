#ifndef LOG_H
#define LOG_H
#include <bits/stdc++.h>

#include "account.h"
using namespace std;

class Log {
   private:
    string file_name_;

   public:
    Log(string file_name);
    ~Log();
    void Record(string rec);
    void ShowFinance(int times);
};

class LogForAll {
   private:
    const string file_name_ = "allog";

   public:
    LogForAll();
    ~LogForAll();
    void Record(string rec);
    void Show();
    void ShowMyself(Account &account);
    void ShowEmployee();
    void ShowTransaction();
    void Clear();
};

#endif