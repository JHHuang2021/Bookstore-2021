#include <cstdio>
#include <cstdlib>
#include <sstream>
#include <stack>
#include <string>

#include "account.h"
#include "error.h"
#include "filemap.hpp"
#include "lib.h"
#include "parser.h"
#define mkpr pair<Account, Book>
vector<mkpr> user_stack;
MainInfo<Account> account_info("account_info");

int main() {
    // freopen("test.in", "r", stdin);
    // freopen("test.out", "w", stdout);
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
            if (user_stack.rend()->first.GetPriority() <= tmp.GetPriority())
                throw Error();
        } else if (tmp.GetPasswd() != password)
            throw Error();
        user_stack.push_back(mkpr(tmp, Book()));
    } else if (token == "logout") {
        if (user_stack.empty()) throw Error();
        user_stack.pop_back();
    }
}
