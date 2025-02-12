#include <bits/stdc++.h>

using namespace std;

class Solution {
public:
    double myPow(double x, int n)
    {
        if (n < 0) {
            return myPowP(1 / x, -n);
        }

        return myPowP(x, n);
    }

    double myPowP(double x, int n)
    {
        if (n == 0) {
            return 1;
        } else {
            return x * myPowP(x, n - 1);
        }
    }
};

int main(int argc, char* argv[])
{
    Solution s = *new Solution();
    cout << s.myPow(0.00001, 2147483647) << endl;
    return 0;
}
