#include <bits/stdc++.h>

// this is bruteforce and won't work
// **TLE**

using namespace std;

class Solution {
public:
    int chalkReplacer(vector<int>& chalk, int k)
    {
        int i = 0;
        int size = chalk.size();
        while (k >= chalk[i]) {
            k -= chalk[i];
            i++;
            i = i % size;
        }
        return i;
    }
};

int main(int argc, char* argv[])
{
    int k = 22;
    vector<int> chalk = { 5, 1, 5 };

    Solution s = *new Solution();

    cout << s.chalkReplacer(chalk, k) << endl;

    return 0;
}
