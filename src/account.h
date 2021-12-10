#ifndef ACCOUNT_H
#define ACCOUNT_H
#include <bits/stdc++.h>

#include <cstring>
using namespace std;  // todo
class Account {
   private:
    char user_id_[30], password_[30], user_name_[30];
    int priority_ = 0;

   public:
    Account();
    Account(string user_id, string user_name, string pw, int priority);
    ~Account();

    Account &operator=(const Account &obj);
    string GetPasswd();
    int GetPriority();

    // void logout();

    void Passwd(string &user_id, string old_password, string new_password);
    void UserAdd(string &user_id, string &password, int priority,
                 string &user_name);
    void Delete(string &user_id);
};

void Register(string &user_id, string &password, string &user_name);
bool Login(string &user_id, string password);

#endif  // ACCOUNT_H