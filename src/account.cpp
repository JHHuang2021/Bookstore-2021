#include "account.h"

#include <cstring>

#include "error.h"
#include "filemap.hpp"

Account::Account() {}

Account::~Account() {}

Account::Account(std::string user_id, std::string user_name, std::string pw, int priority) {
    strcpy(this->user_id_, user_id.c_str());
    strcpy(this->password_, pw.c_str());
    strcpy(this->user_name_, user_name.c_str());
    this->priority_ = priority;
}

Account &Account::operator=(const Account &obj) {
    strcpy(this->user_id_, obj.user_id_);
    strcpy(this->password_, obj.password_);
    strcpy(this->user_name_, obj.user_name_);
    this->priority_ = obj.priority_;
    return *this;
}

std::string Account::GetPasswd() { return this->password_; }
std::string Account::GetUserId() { return this->user_id_; }
int Account::GetPriority() { return this->priority_; }

void Register(std::string &user_id, std::string &password, std::string &user_name) {
    MainInfo<Account> account_info("account_info");
    try {
        account_info.FindInfo(user_id);
        throw Error();
    } catch (Error &ex) {
        Account tmp(user_id, user_name, password, 1);
        account_info.WriteInfo(tmp, user_id);
    }
}

bool Login(std::string &user_id, std::string password = "") {
    if (password == "") return true;
    MainInfo<Account> account_info("account_info");
    Account tmp = account_info.FindInfo(user_id);  // copy constructor
    if (tmp.GetPasswd() == password) return true;
    return false;
}

void Passwd(std::string &user_id, std::string new_password, int priority,
            std::string old_password) {
    MainInfo<Account> account_info("account_info");
    // note!!! may slower the program
    Account tmp = account_info.FindInfo(user_id);
    if ((priority == 7 && old_password == "") ||
        strcmp(old_password.c_str(), tmp.password_) == 0) {
        strcpy(tmp.password_, new_password.c_str());
        account_info.DeleteInfo(user_id);
        account_info.WriteInfo(tmp, user_id);
    } else
        throw Error();
}

void Delete(std::string &user_id) {
    MainInfo<Account> account_info("account_info");
    account_info.DeleteInfo(user_id);
}

void Account::UserAdd(std::string &user_id, std::string &password, int priority,
                      std::string &user_name) {
    if (this->priority_ <= priority) throw Error();
    MainInfo<Account> account_info("account_info");
    try {
        account_info.FindInfo(user_id);
        throw Error();
    } catch (Error &ex) {
        Account tmp(user_id, user_name, password, priority);
        account_info.WriteInfo(tmp, user_id);
    }
}
