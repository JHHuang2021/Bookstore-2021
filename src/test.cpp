#include <bits/stdc++.h>
using namespace std;

int main() {
    const int kScale = 2000;
    cout << 2 * kScale + 3 << endl;
    for (int i = 1000; i >= 1000 - kScale; i--)
        cout << "insert ACM " << i << endl;
    for (int i = -1000; i <= -1000 + kScale; i++)
        cout << "delete ACM " << i << endl;
    cout << "find ACM";
    return 0;
}