#ifndef LOG_H
#define LOG_H
#include <bits/stdc++.h>

#include "account.h"

class Log {
   private:
    std::string file_name_;

   public:
    Log(std::string file_name);
    ~Log();
    void Record(std::string rec);
    void ShowFinance(int times);
};

class LogForAll {
   private:
    const std::string file_name_ = "allog";

   public:
    LogForAll();
    ~LogForAll();
    void Record(std::string rec);
    void Show();
    void ShowMyself(Account &account);
    void ShowEmployee();
    void ShowTransaction();
    void Clear();
};

#endif