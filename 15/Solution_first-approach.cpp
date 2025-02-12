#include <bits/stdc++.h>

using namespace std;

class Solution {
public:
    Solution()
    {
        ios_base::sync_with_stdio(false);
        cin.tie(nullptr);
        cout.tie(nullptr);
    }

    vector<vector<int>> threeSum(vector<int>& nums)
    {
        int N = nums.size();
        sort(nums.begin(), nums.end());

        vector<vector<int>> res = {};

        int start = 0;
        int middle = N / 2;
        if (N % 2 == 0) {
            middle--;
        }
        int end = N - 1;

        while (start < middle && middle < end) {
            // first skip the dupes
            while (start < middle && nums[start] == nums[start + 1])
                start++;

            while (end > middle && nums[end] == nums[end - 1])
                end--;

            int first = nums[start];
            int second = nums[middle];
            int third = nums[end];

            int sum = first + second + third;

            if (sum == 0) {
                res.push_back({ first, second, third });
                middle++;
            } else if (sum < 0) {
                start++;
            } else if (sum > 0) {
                end--;
            }
        }

        return res;
    }
};
