#include <bits/stdc++.h>

using namespace std;

class Solution {
public:
    double myPow(double x, int n)
    {
        double ans = 1;
        int ogN = n;

        if (n < 0) {
            x = 1 / x;
            n = -n;
        }

        while (n) {
            if (n % 2) {
                ans *= x;
                n--;
            } else {
                x = x * x;
                n = n / 2;
            }
        }

        if (ogN % 2 && x < 0) {
            return -ans;
        }

        return ans;
    }
};

int main(int argc, char* argv[])
{
    Solution s = *new Solution();
    cout << s.myPow(2, 3) << endl;
    return 0;
}
