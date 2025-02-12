#include <bits/stdc++.h>

using namespace std;

class Solution {
public:
    int maxProfit(vector<int>& prices)
    {
        int N = prices.size();
        int low = prices[0];
        int high = prices[0];
        int max_diff = high - low; // 0 in the start

        int high_day = 0;
        int low_day = 0;

        for (int i = 1; i < N; i++) {
            if (prices[i] < low) {
                low_day = i;
                low = prices[i];
            }

            if (prices[i] > high) {
                high_day = i;
                high = prices[i];
            }

            if (high_day < low_day) {
                high = low;
            }
            int t = high - low;
            max_diff = max(t, max_diff);
        }
        return max_diff;
    }
};

int main(int argc, char* argv[])
{
    Solution s = *new Solution();
    vector<int> v = { 7, 6, 4, 3, 1 };
    cout << s.maxProfit(v) << endl;
    return 0;
}
