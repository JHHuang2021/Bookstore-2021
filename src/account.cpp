#include "account.h"

#include <cstring>

#include "error.h"
#include "filemap.hpp"

Account::Account() {}

Account::~Account() {}

Account::Account(string &user_id, string &user_name, string &pw, int priority) {
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

string Account::GetPasswd() { return this->password_; }

void Register(string &user_id, string &password, string &user_name) {
    MainInfo<Account> account_info("account_info");
    try {
        account_info.FindInfo(user_id);
        throw Error();
    } catch (Error &ex) {
        account_info.WriteInfo(Account(user_id, user_name, password, 1),
                               user_id);
    }
}

bool Login(string &user_id, string password = "") {
    if (password == "") return true;
    MainInfo<Account> account_info("account_info");
    Account tmp = account_info.FindInfo(user_id);  // copy constructor
    if (tmp.GetPasswd() == password) return true;
    return false;
}