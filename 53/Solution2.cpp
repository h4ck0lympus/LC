#include <bits/stdc++.h>

using namespace std;

class Solution {
public:
    int maxSubArray(vector<int>& nums)
    {
        int N = nums.size();
        int current_sum = nums[0];
        int max_sum = nums[0];

        for (int i = 1; i < N; i++) {
            int t = current_sum + nums[i];

            current_sum = max(nums[i], t);
            max_sum = max(current_sum, max_sum);
        }

        return max_sum;
    }
};

int main(int argc, char* argv[])
{
    Solution s = *new Solution();

    vector<int> v = { -2, 1, -3, 4, -1, 2, 1, -5, 4 };
    int x = s.maxSubArray(v);
    cout << x << endl;
    return 0;
}
