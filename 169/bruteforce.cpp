#include <bits/stdc++.h>

using namespace std;

class Solution {
public:
    int majorityElement(vector<int>& nums)
    {
        int N = nums.size();
        map<int, int> record;

        int maxCount = 0;
        int maxElement = 0;

        for (int num : nums) {
            record[num]++;
            if (record[num] > maxCount) {
                maxCount = record[num];
                maxElement = num;
            }
        }

        return maxElement;
    }
};
