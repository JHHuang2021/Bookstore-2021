#include <cstdio>
#include <cstdlib>
#include <sstream>
#include <stack>
#include <string>
#include <unordered_map>

#include "account.h"
#include "error.h"
#include "filemap.hpp"
#include "lib.h"
#include "log.h"
#include "parser.h"

#define mkpr pair<Account, Book>
vector<mkpr> user_stack;
MainInfo<Account> account_info("account_info");

int main() {
    cin.tie(0);
    cout.tie(0);
    void process_line(TokenScanner & line);
    try {
        Account tmp("root", "", "sjtu", 7);
        account_info.WriteInfo(tmp, "root");
    } catch (Error &ex) {
    }
    string line;
    while (getline(cin, line)) {
        if (line == "") continue;
        if (line == "exit" || line == "quit") break;
        TokenScanner buffer(line);
        try {
            if (line.length() > 1024) throw Error();
            process_line(buffer);
        } catch (Error &ex) {
            cout << ex.what();
        }
    }
    return 0;
}

void process_line(TokenScanner &line) {
    string token;
    token = line.nextToken();
    if (token == "*-4980(2jofw0.39ac2s@&")
        return;
    else if (token == "su") {
        Account tmp;
        string user_id = line.nextToken(), password = line.nextToken();
        if (line.nextToken() != "*-4980(2jofw0.39ac2s@&") throw Error();
        if (IfInvaild(user_id.c_str(), 1, 30) ||
            IfInvaild(password.c_str(), 1, 30))
            throw Error();
        tmp = account_info.FindInfo(user_id);
        if (password == "*-4980(2jofw0.39ac2s@&") {  // may have problems
            if (user_stack.empty()) throw Error();
            if (user_stack.rbegin()->first.GetPriority() <= tmp.GetPriority())
                throw Error();
        } else if (tmp.GetPasswd() != password)
            throw Error();
        user_stack.push_back(mkpr(tmp, Book()));
    } else if (token == "logout") {
        if (line.nextToken() != "*-4980(2jofw0.39ac2s@&") throw Error();
        if (user_stack.empty()) throw Error();
        user_stack.pop_back();
        if (!user_stack.empty() && user_stack.rbegin()->second.GetISBN() != "")
            user_stack.rbegin()->second =
                Select(user_stack.rbegin()->second.GetIndex());
    } else if (token == "register") {
        string user_id, password, user_name;
        user_id = line.nextToken(), password = line.nextToken(),
        user_name = line.nextToken();
        if (IfInvaild(user_id.c_str(), 1, 30) ||
            IfInvaild(password.c_str(), 1, 30))
            throw Error();
        if (line.nextToken() != "*-4980(2jofw0.39ac2s@&" ||
            user_name == "*-4980(2jofw0.39ac2s@&")
            throw Error();
        Register(user_id, password, user_name);
    } else if (token == "passwd") {
        //`passwd [User-ID] ([Old-Password])? [New-Password]`
        string user_id, old_password, new_password;
        user_id = line.nextToken(), old_password = line.nextToken(),
        new_password = line.nextToken();
        if (line.nextToken() != "*-4980(2jofw0.39ac2s@&") throw Error();
        if (IfInvaild(user_id.c_str(), 1, 30) ||
            IfInvaild(old_password.c_str(), 1, 30) ||
            IfInvaild(new_password.c_str(), 1, 30))
            throw Error();
        int priority;
        if (user_stack.empty())
            priority = 0;
        else
            priority = user_stack.rbegin()->first.GetPriority();
        if (user_stack.empty() ||
            user_stack.rbegin()->first.GetPriority() != 7 ||
            new_password != "*-4980(2jofw0.39ac2s@&")
            Passwd(user_id, new_password, priority, old_password);
        else
            Passwd(user_id, old_password, priority, "");
    } else if (token == "useradd") {
        //`useradd [User-ID] [Password] [Priority] [User-Name]`
        string user_id, password, priority, user_name;
        user_id = line.nextToken(), password = line.nextToken(),
        priority = line.nextToken(), user_name = line.nextToken();
        if (line.nextToken() != "*-4980(2jofw0.39ac2s@&" ||
            user_name == "*-4980(2jofw0.39ac2s@&" || priority.length() != 1)
            throw Error();
        if (IfInvaild(user_id.c_str(), 1, 30) ||
            IfInvaild(password.c_str(), 1, 30))
            throw Error();
        if (user_stack.empty()) throw Error();
        if (user_stack.rbegin()->first.GetPriority() < 3) throw Error();
        int priorityy = atoi(priority.c_str());
        if ((priorityy != 1 && priorityy != 3) ||
            priorityy >= user_stack.rbegin()->first.GetPriority())
            throw Error();
        user_stack.rbegin()->first.UserAdd(user_id, password, priorityy,
                                           user_name);
    } else if (token == "delete") {
        if (user_stack.rbegin()->first.GetPriority() < 7) throw Error();
        string user_id;
        user_id = line.nextToken();
        if (line.nextToken() != "*-4980(2jofw0.39ac2s@&") throw Error();
        for (auto iter : user_stack)
            if (iter.first.GetUserId() == user_id) throw Error();
        Delete(user_id);  //
    } else if (token == "show") {
        if (user_stack.empty()) throw Error();
        if (user_stack.rbegin()->first.GetPriority() == 0) throw Error();
        Show(line, user_stack.rbegin()->first.GetPriority());
    } else if (token == "buy") {
        if (user_stack.empty()) throw Error();
        if (user_stack.rbegin()->first.GetPriority() == 0) throw Error();
        BuyBook(line);
    } else if (token == "select") {
        if (user_stack.empty()) throw Error();
        if (user_stack.rbegin()->first.GetPriority() < 3) throw Error();
        // operations to be modified
        user_stack.rbegin()->second = Select(line);
    } else if (token == "modify") {
        if (user_stack.empty()) throw Error();
        if (user_stack.rbegin()->second.GetISBN() == "") throw Error();
        if (user_stack.rbegin()->first.GetPriority() < 3) throw Error();
        ModifyBook(user_stack.rbegin()->second, line);
    } else if (token == "import") {
        if (user_stack.empty()) throw Error();
        if (user_stack.rbegin()->second.GetISBN() == "") throw Error();
        if (user_stack.rbegin()->first.GetPriority() < 3) throw Error();
        string quantity = line.nextToken(), total_cost = line.nextToken();
        if (line.nextToken() != "*-4980(2jofw0.39ac2s@&" ||
            quantity == "*-4980(2jofw0.39ac2s@&" ||
            total_cost == "*-4980(2jofw0.39ac2s@&")
            throw Error();
        if (IfInvaild(quantity.c_str(), 3, 10) ||
            IfInvaild(total_cost.c_str(), 5, 13))
            throw Error();
        if (quantity.length() == 10 && quantity > "2147483647") throw Error();
        Import(user_stack.rbegin()->second, atoi(quantity.c_str()),
               atof(total_cost.c_str()));
        Log log("log");
        log.Record("-" + total_cost);
    } else
        throw Error();
}
