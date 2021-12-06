#include <bits/stdc++.h>

#include <cstdio>
#include <cstdlib>
#include <sstream>
#include <string>

#include "unrolled_linklist.hpp"
int main() {
    // freopen("test.in", "r", stdin);
    // freopen("test.out", "w", stdout);
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

    // Initialize ULL with file name
    Ull testUll("test.dat", "test_free.dat");

    // Save current time(hhmmss as integer) to file
    // set<int> array;
    // for (int i = 1; i <= 1; i++) {
    //     testUll.addNode(UllNode("CppPrimer", i));
    // }
    // testUll.findNode("CppPrimer", array);
    // // testUll.show();
    // for (int i = 1; i <= 1; i++) {
    //     testUll.deleteNode(UllNode("CppPrimer", i));
    // }
    // testUll.deleteNode(UllNode("CppPrimer1", 1));

    // for (int i = 1; i <= 150; i++) {
    //     testUll.deleteNode(UllNode("CppPrimer", i));
    // }

    // for (int i = 201; i <= 400; i++) testUll.addNode(UllNode("CppPrimer",
    // i));
    // testUll.findNode("CppPrimer", array);
    // while (!array.empty()) {
    //     cout << *array.begin() << " ";
    //     array.erase(array.begin());
    // }
    // cout << endl;
    // Print all entries in file.
    // std::vector<int> retVec;
    // testUll.findNode("Bello ACM!", retVec);
    // for (auto item : retVec) std::cout << item << std::endl;

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
            set<int> find;
            find.clear();
            in >> str1;
            testUll.findNode(str1, find);
            while (!find.empty()) {
                auto iter = find.find(0);
                if (iter != find.end()) find.erase(iter);
            }
            if (find.empty())
                cout << "null" << endl;
            else {
                while (!find.empty()) {
                    cout << *find.begin() << " ";
                    find.erase(find.begin());
                }
                cout << '\n';
            }
        }
        if (str == "delete") {
            string str1, str2;
            in >> str1 >> str2;
            testUll.deleteNode(UllNode(str1, atoi(str2.c_str())));
        }
    }
    return 0;
}
