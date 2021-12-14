#ifndef LOG_H
#define LOG_H
#include <bits/stdc++.h>
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

#endif