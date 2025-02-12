#include <bits/stdc++.h>

using namespace std;

typedef long long ll;

class Solution {
public:
    Solution()
    {
        ios_base::sync_with_stdio(false);
        cin.tie(NULL);
        cout.tie(NULL);
    }

    ll chalkReplacer(vector<int>& chalk, int k)
    {
        // for (ll i : chalk) {
        //     sum += i;
        // }
        // using STL for better speed
        ll sum = 0;
        sum = accumulate(chalk.begin(), chalk.end(), sum);
        k = k % sum;
        ll idx = 0;
        ll size = chalk.size();

        while (k > chalk[idx]) {
            k = k - chalk[idx];
            idx++;
        }

        return idx;
    }
};

int main(int argc, char* argv[])
{
    ll k = 22;
    vector<int> chalk = { 5, 1, 5 };
    Solution s = *new Solution();
    cout << s.chalkReplacer(chalk, k) << endl;
    return 0;
}
