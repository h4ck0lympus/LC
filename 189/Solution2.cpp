#include <bits/stdc++.h>

using namespace std;

class Solution {
public:
    void rotate(vector<int>& nums, int k)
    {
        k = k % nums.size();
        // reverse everything
        for (int i = 0, j = nums.size() - 1; i < j; i++, j--) {
            int temp = nums[i];
            nums[i] = nums[j];
            nums[j] = temp;
        }

        // reverse first k elements
        int first = 0;
        int last = k - 1;

        while (first <= last) {
            int temp = nums[first];
            nums[first] = nums[last];
            nums[last] = temp;

            first++;
            last--;
        }

        // reverse remaining elements
        first = k;
        last = nums.size() - 1;

        while (first <= last) {
            int temp = nums[first];
            nums[first] = nums[last];
            nums[last] = temp;

            first++;
            last--;
        }
    }
};
