#include <bits/stdc++.h>

#include <cstdio>
#include <cstdlib>
#include <sstream>
#include <stack>
#include <string>

#include "account.h"
#include "lib.h"
#include "parser.h"
#include "unrolled_linklist.h"
#define mkpr pair<account, book>
Ull user_info("user_info"), book_info("book_info");

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
    stack<mkpr> user_stack;
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
        string user_name = line.nextToken(), password = line.nextToken();
    }
}
