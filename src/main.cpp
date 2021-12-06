#include <bits/stdc++.h>

#include <cstdio>
#include <cstdlib>
#include <sstream>
#include <string>

#include "unrolled_linklist.hpp"
int main() {
    freopen("test.in", "r", stdin);
    freopen("test.out", "w", stdout);
    // ! Notice
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

    Ull testUll("test.dat", "test_free.dat");
    int n;
    cin >> n;
    getchar();
    for (int i = 1; i <= n; i++) {
        string str;
        getline(cin, str);
        istringstream in(str);
        in >> str;
        if (str == "insert") {
            string str1, str2;
            in >> str1 >> str2;
            testUll.addNode(UllNode(str1, atoi(str2.c_str())));
        }
        if (str == "find") {
            string str1;
            set<int> tmp;
            tmp.clear();
            in >> str1;
            testUll.findNode(str1, tmp);
            if (tmp.empty())
                cout << "null";
            else
                while (!tmp.empty()) {
                    cout << *tmp.begin() << " ";
                    tmp.erase(tmp.begin());
                }
            cout << '\n';
        }
        if (str == "delete") {
            string str1, str2;
            in >> str1 >> str2;
            testUll.deleteNode(UllNode(str1, atoi(str2.c_str())));
        }
        // testUll.show();
    }
    return 0;
}
