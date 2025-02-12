/**
 * @author      : HackOlympus (zeus@hackolympus)
 * @file        : Solution
 * @created     : Sunday Mar 12, 2023 01:48:08 MST
 */

#include <bits/stdc++.h>
#include <vector>

using namespace std;

class Solution {
public:
    int leftSum(vector<int>& nums)
    {
        int sum = 0;
        int size = nums.size();
        for (int i = 0; i < size / 2; i++) {
            sum += nums[i];
        }

        return sum;
    }

    int rightSum(vector<int>& nums)
    {
        int sum = 0;
        int size = nums.size();
        for (int i = (size / 2) + 1; i < size; i++) {
            sum += nums[i];
        }

        return sum;
    }

    int pivotIndex(vector<int>& nums)
    {
        int length = nums.size();
        int LS = leftSum(nums);
        int RS = rightSum(nums);

        int M = length / 2;
        int L = M;
        int R = M;

        if (LS == RS) {
            return M;
        }

        int LS1 = LS, LS2 = LS;
        int RS1 = RS, RS2 = RS;

        while (L >= 0 && R <= length) {
            // we go one left
            LS1 -= nums[L];
            RS1 += nums[R];



            L--;
            R++;
        }

        return -1;
    }
};

int main()
{
    Solution soln;
    vector<int> v = { 2, 1, -1 };
    cout << soln.pivotIndex(v) << endl;
    return 0;
}
