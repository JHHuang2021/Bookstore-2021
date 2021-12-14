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
#include "parser.h"

#define mkpr pair<Account, Book>
// may use struct
vector<mkpr> user_stack;
MainInfo<Account> account_info("account_info");

int main() {
    // If you are using dynamic-link library, the DLL
    // file (filename extension is `.so` in Linux and
    // `.dll` in Windows) must be in a location that
    // the executable file can directly access(that is,
    // under the same folder or under system PATH),
    // otherwise it cannot run.
    // But if you are using the statically-linked library
    // (filename extension is `.a` in Linux and `.lib`
    // in Windows), the executable file can run without
    // any other file.
    freopen("test.in", "r", stdin);
    freopen("test.out", "w", stdout);
    void process_line(TokenScanner & line);
    try {
        Account tmp("root", "", "sjtu", 7);
        account_info.WriteInfo(tmp, "root");
    } catch (Error &ex) {
    }

    while (true) {
        string line;
        getline(cin, line);
        if (line == "") break;
        TokenScanner buffer(line);
        try {
        } catch (string info) {
            cout << info << endl;
        }
    }
    return 0;
}

void process_line(TokenScanner &line) {
    string token;
    token = line.nextToken();
    if (token == "-1") return;
    if (token == "su") {
        Account tmp;
        string user_name = line.nextToken(), password = line.nextToken();
        tmp = account_info.FindInfo(user_name);
        if (password == "") {
            if (user_stack.empty()) throw Error();
            if (user_stack.rbegin()->first.GetPriority() <= tmp.GetPriority())
                throw Error();
        } else if (tmp.GetPasswd() != password)
            throw Error();
        user_stack.push_back(mkpr(tmp, Book()));
    } else if (token == "logout") {
        if (user_stack.empty()) throw Error();
        user_stack.pop_back();
    } else if (token == "register") {
        string user_id, password, user_name;
        user_id = line.nextToken(), password = line.nextToken(),
        user_name = line.nextToken();
        Register(user_id, password, user_name);
    } else if (token == "passwd") {
        //`passwd [User-ID] ([Old-Password])? [New-Password]`
        string user_id, old_password, new_password;
        user_id = line.nextToken(), old_password = line.nextToken(),
        new_password = line.nextToken();
        int priority;
        if (user_stack.empty())
            priority = 0;
        else
            priority = user_stack.rbegin()->first.GetPriority();
        if (user_id != "root")
            Passwd(user_id, new_password, priority, old_password);
        else
            Passwd(user_id, old_password, priority, "");
    } else if (token == "useradd") {
        //`useradd [User-ID] [Password] [Priority] [User-Name]`
        string user_id, password, priority, user_name;
        user_id = line.nextToken(), password = line.nextToken(),
        priority = line.nextToken(), user_name = line.nextToken();
        if (user_stack.empty()) throw Error();
        user_stack.rbegin()->first.UserAdd(user_id, password,
                                           atoi(priority.c_str()), user_name);
    } else if (token == "delete") {
        if (user_stack.rbegin()->first.GetPriority() < 3) throw Error();
        string user_id;
        user_id = line.nextToken();
        for (auto iter : user_stack)
            if (iter.first.GetUserId() == user_id) throw Error();
        Delete(user_id);
    } else if (token == "show") {
        if (user_stack.empty()) throw Error();
        if (user_stack.rbegin()->first.GetPriority() == 0) throw Error();
        Show(line);
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
        Import(user_stack.rbegin()->second, atoi(line.nextToken().c_str()),
               atof(line.nextToken().c_str()));
    }
}
