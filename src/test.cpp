#include <bits/stdc++.h>
using namespace std;

int main() {
    const int kScale = 2000;
    cout << 2 * kScale << endl;
    for (int i = 1000; i >= 1000 - kScale; i--)
        cout << "insert ACM " << i << endl;
    for (int i = -1000; i <= -1000 + kScale; i++)
        cout << "insert JOHN " << i << endl;
    return 0;
}