#include <netinet/in.h>
#include <sys/socket.h>

#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <cstring>
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

string Process(string cmd) {
    // freopen("test.in", "r", stdin);
    // cin.tie(0);
    // cout.tie(0);
    string process_line(TokenScanner & line);
    if (cmd == "") return "";
    // if (line == "exit" || line == "quit") break;
    TokenScanner buffer(cmd);
    try {
        for (int i = 0; i < cmd.length(); i++)
            if ((cmd[i] < 32 && cmd[i] != 10 && cmd[i] != 13) ||
                cmd[i] >= 127 || !isascii(cmd[i]))
                throw Error();
        if (cmd.length() > 1024) throw Error();
        return process_line(buffer);
    } catch (Error &ex) {
        return ex.what();
    }
}

string process_line(TokenScanner &line) {
    string token;
    token = line.nextToken();
    if (token == "*-4980(2jofw0.39ac2s@&")
        return "";
    else if (token == "exit" || token == "quit") {
        if (line.nextToken() != "*-4980(2jofw0.39ac2s@&")
            throw Error();
        else
            exit(0);
    } else if (token == "su") {
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
        return "1";
    } else if (token == "logout") {
        if (line.nextToken() != "*-4980(2jofw0.39ac2s@&") throw Error();
        if (user_stack.empty()) throw Error();
        user_stack.pop_back();
        if (!user_stack.empty() && user_stack.rbegin()->second.GetISBN() != "")
            user_stack.rbegin()->second =
                Select(user_stack.rbegin()->second.GetIndex());
        return "1";
    } else if (token == "register") {
        string user_id, password, user_name;
        user_id = line.nextToken(), password = line.nextToken(),
        user_name = line.nextToken();
        if (IfInvaild(user_id.c_str(), 1, 30) ||
            IfInvaild(password.c_str(), 1, 30) ||
            IfInvaild(password.c_str(), 2, 30))
            throw Error();
        if (line.nextToken() != "*-4980(2jofw0.39ac2s@&" ||
            user_name == "*-4980(2jofw0.39ac2s@&")
            throw Error();
        Register(user_id, password, user_name);
        return "1";
    } else if (token == "passwd") {
        //`passwd [User-ID] ([Old-Password])? [New-Password]`
        string user_id, old_password, new_password;
        user_id = line.nextToken(), old_password = line.nextToken(),
        new_password = line.nextToken();
        if (user_stack.empty()) throw Error();
        if (line.nextToken() != "*-4980(2jofw0.39ac2s@&") throw Error();
        if (old_password == "*-4980(2jofw0.39ac2s@&" &&
            new_password == "*-4980(2jofw0.39ac2s@&")
            throw Error();
        if (old_password != "*-4980(2jofw0.39ac2s@&" &&
            new_password == "*-4980(2jofw0.39ac2s@&" &&
            (user_stack.empty() ||
             user_stack.rbegin()->first.GetPriority() != 7))
            throw Error();
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
        return "1";
    } else if (token == "useradd") {
        //`useradd [User-ID] [Password] [Priority] [User-Name]`
        string user_id, password, priority, user_name;
        user_id = line.nextToken(), password = line.nextToken(),
        priority = line.nextToken(), user_name = line.nextToken();
        if (line.nextToken() != "*-4980(2jofw0.39ac2s@&" ||
            user_name == "*-4980(2jofw0.39ac2s@&" || priority.length() != 1)
            throw Error();
        if (IfInvaild(user_id.c_str(), 1, 30) ||
            IfInvaild(password.c_str(), 1, 30) ||
            IfInvaild(password.c_str(), 2, 30))
            throw Error();
        if (user_stack.empty()) throw Error();
        if (user_stack.rbegin()->first.GetPriority() < 3) throw Error();
        int priorityy = atoi(priority.c_str());
        if ((priorityy != 1 && priorityy != 3) ||
            priorityy >= user_stack.rbegin()->first.GetPriority())
            throw Error();
        user_stack.rbegin()->first.UserAdd(user_id, password, priorityy,
                                           user_name);
        return "1";
    } else if (token == "delete") {
        if (user_stack.empty() || user_stack.rbegin()->first.GetPriority() < 7)
            throw Error();
        string user_id;
        user_id = line.nextToken();
        if (line.nextToken() != "*-4980(2jofw0.39ac2s@&") throw Error();
        for (auto iter : user_stack)
            if (iter.first.GetUserId() == user_id) throw Error();
        Delete(user_id);  //
        return "1";
    } else if (token == "show") {  ////////
        if (user_stack.empty()) throw Error();
        if (user_stack.rbegin()->first.GetPriority() == 0) throw Error();
        return Show(line, user_stack.rbegin()->first.GetPriority()).str();
    } else if (token == "buy") {
        if (user_stack.empty()) throw Error();
        if (user_stack.rbegin()->first.GetPriority() == 0) throw Error();
        BuyBook(line);
        return "1";
    } else if (token == "select") {  ////////
        if (user_stack.empty()) throw Error();
        if (user_stack.rbegin()->first.GetPriority() < 3) throw Error();
        // operations to be modified
        user_stack.rbegin()->second = Select(line);
    } else if (token == "modify") {
        if (user_stack.empty()) throw Error();
        if (user_stack.rbegin()->second.GetISBN() == "") throw Error();
        if (user_stack.rbegin()->first.GetPriority() < 3) throw Error();
        ModifyBook(user_stack.rbegin()->second, line);
        return "1";
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
        return "1";
    } else
        throw Error();
}

void RunFrontEnd() {
    const int BUFFER_SIZE = 102400;
    int slisten = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in sin;
    sin.sin_family = AF_INET;
    sin.sin_port = htons(7777);
    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(slisten, (struct sockaddr *)&sin, sizeof(sin)) == -1)
        printf("bind error!");
    if (listen(slisten, 5) == -1) {
        printf("listen error!");
        exit(0);
    }
    int sclient;
    sockaddr_in client_add;
    socklen_t naddrlen = sizeof(client_add);
    char revdata[BUFFER_SIZE], command[BUFFER_SIZE], send_result[BUFFER_SIZE];

    while (1) {
        memset(command, 0, sizeof(command));
        memset(revdata, 0, sizeof(revdata));
        sclient = accept(slisten, (struct sockaddr *)&client_add, &naddrlen);
        if (sclient == -1) {
            printf("Socket:Accept Error!");
            continue;
        }
        recv(sclient, revdata, BUFFER_SIZE, 0);
        cout << "recieve command: " << revdata << " length: " << strlen(revdata)
             << endl;
        string result_str = Process(revdata);
        send(sclient, result_str.c_str(), result_str.length(), 0);
    }
}

int main() {
    try {
        Account tmp("root", "", "sjtu", 7);
        account_info.WriteInfo(tmp, "root");
    } catch (Error &ex) {
    }
    RunFrontEnd();
    return 0;
}