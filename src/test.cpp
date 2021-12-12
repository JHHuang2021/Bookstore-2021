#include <bits/stdc++.h>
using namespace std;

int main() {
    const int kScale = 10000;
    cout << 2 * kScale << endl;
    for (int i = 5000; i >= 5000 - kScale; i--)
        cout << "insert ACM " << i << endl;
    for (int i = -5000; i <= -5000 + kScale; i++)
        cout << "insert JOHN " << i << endl;
    return 0;
}