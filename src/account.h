#ifndef ACCOUNT_H
#define ACCOUNT_H
#include <bits/stdc++.h>

#include <cstring>
class Account {
    friend void Passwd(std::string &user_id, std::string old_password, int priority,
                       std::string new_password);

   private:
    char user_id_[31], password_[31], user_name_[31];
    int priority_ = 0;

   public:
    Account();
    Account(std::string user_id, std::string user_name, std::string pw, int priority);
    ~Account();

    Account &operator=(const Account &obj);
    std::string GetPasswd();
    std::string GetUserId();
    int GetPriority();

    // void logout();

    void UserAdd(std::string &user_id, std::string &password, int priority,
                 std::string &user_name);
};
void Passwd(std::string &user_id, std::string new_password, int priority,
            std::string old_password);
void Delete(std::string &user_id);
void Register(std::string &user_id, std::string &password, std::string &user_name);
bool Login(std::string &user_id, std::string password);

#endif  // ACCOUNT_H