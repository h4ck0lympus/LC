#include <bits/stdc++.h>
using namespace std;

/*
Given an array nums of size n and an integer k, find the length of the longest sub-array that sums to k. If no such sub-array exists, return 0.

https://takeuforward.org/plus/dsa/problems/longest-subarray-with-sum-k

*/

class Solution{
public:
    int longestSubarray(vector<int> &nums, int k){
        int cs = 0; // current sum
        int cl = 0; // current length
        int ml = 0; // max length

        for (int i=0; i < nums.size(); i++) {
            cs += nums[i];
            cl++;

            if (cs >= k) {
                ml = max(cl, ml);
                // reset cs to current nums[i]
                cs = nums[i];
                cl = 1;
            }
        }

        return ml; // return max length
    }
};

int main()
{
    Solution s = *new Solution();
    // vector<int> nums = {10, 5, 2, 7, 1, 9};
    vector<int> nums = {1, 2, 1, 2, 1};
    int k = 3;
    cout << s.longestSubarray(nums, k) << endl;
    return 0;
}
