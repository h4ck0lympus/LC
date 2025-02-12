#include <bits/stdc++.h>

using namespace std;

/*
 * sum of any 2 sides must be greater than the third side
 */

class Solution {
public:
    Solution()
    {
        ios_base::sync_with_stdio(false);
        cin.tie(nullptr);
        cout.tie(nullptr);
    }

    int largestPerimeter(vector<int>& nums)
    {
        sort(nums.begin(), nums.end());

        int N = nums.size();

        int pmtr = 0;

        int max1 = 0;
        int max2 = 0;
        int max3 = 0;

        for (int i = N - 1; i >= 2; i--) {
            int side1 = nums[i];
            int side2 = nums[i - 1];
            int side3 = nums[i - 2];

            if (side1 + side2 <= side3) {
                continue;
            }

            if (side1 + side3 <= side2) {
                continue;
            }

            if (side2 + side3 <= side1) {
                continue;
            }

            pmtr = max(pmtr, side1 + side2 + side3);
        }

        return pmtr;
    }
};
