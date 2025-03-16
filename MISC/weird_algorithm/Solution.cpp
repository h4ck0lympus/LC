#include <bits/stdc++.h>

using namespace std;

void Solution(long long n)
{
    while (n != 1) {
        cout << n << " ";
        if (n & 1) {
            n = 3 * n + 1;
        } else {
            n = n / 2;
        }
    }
    cout << 1;
}

int main (int argc, char *argv[]) {
    long long n; cin >> n;
    Solution(n);
    return 0;
}
