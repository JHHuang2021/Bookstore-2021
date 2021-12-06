#include <bits/stdc++.h>

#include <utility>
using namespace std;
#define mkpr pair<string, int>
int main() {
    set<mkpr> mp;
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
            mp.insert(mkpr(str1, atoi(str2.c_str())));
        }
        if (str == "find") {
            string str1;
            set<int> find;
            in >> str1;
            for (auto iter : mp)
                if (iter.first == str1) find.insert(iter.second);
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
            auto iter = mp.find(mkpr(str1, atoi(str2.c_str())));
            if (iter != mp.end()) mp.erase(iter);
        }
    }
    return 0;
}