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
    // freopen("test.in", "r", stdin);
    // freopen("test.out", "w", stdout);
    int n;
    cin >> n;
    getchar();
    Ull test("test.dat");
    string str;
    for (int i = 1; i <= n; i++) {
        getline(cin, str);
        istringstream ss(str);
        string token;
        ss >> token;
        if (token == "insert") {
            string index, value;
            ss >> index >> value;
            test.AddNode(UllNode(index, atoi(value.c_str())));
        } else if (token == "delete") {
            string index, value;
            ss >> index >> value;
            test.DeleteNode(UllNode(index, atoi(value.c_str())));
        } else if (token == "find") {
            set<int> find;
            string index;
            ss >> index;
            test.FindNode(index, find);
            if (find.empty())
                cout << "null" << endl;
            else {
                while (!find.empty()) {
                    cout << *find.begin() << " ";
                    find.erase(find.begin());
                }
                cout << "\n";
            }
        }
    }

    // void process_line(TokenScanner & line);
    // try {
    //     Account tmp("root", "", "sjtu", 7);
    //     account_info.WriteInfo(tmp, "root");
    // } catch (Error &ex) {
    // }

    // while (true) {
    //     string line;
    //     getline(cin, line);
    //     if (line == "") break;
    //     TokenScanner buffer(line);
    //     try {
    //     } catch (string info) {
    //         cout << info << endl;
    //     }
    // }
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
            priority = user_stack.rend()->first.GetPriority();
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
        user_stack.rend()->first.UserAdd(user_id, password,
                                         atoi(priority.c_str()), user_name);
    } else if (token == "delete") {
        string user_id;
        user_id = line.nextToken();
        for (auto iter : user_stack)
            if (iter.first.GetUserId() == user_id) throw Error();
        Delete(user_id);
    }
}
