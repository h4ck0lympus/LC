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
    int sumOfArr(vector<int>& nums)
    {
        int sum = 0;
        for (int num : nums) {
            sum += num;
        }
        return sum;
    }

    int pivotIndex(vector<int>& nums)
    {
        int N = nums.size();

        if (N == 1) {
            return 0;
        }

        if (N == 2 || N == 0) {
            return -1;
        }

        int pivot = N - 1;
        int sum = sumOfArr(nums);
        int LS = sum, RS = 0;

        while (pivot >= 0) {
            LS -= nums[pivot];
            if (LS == RS) {
                return pivot;
            }
            RS += nums[pivot];
            pivot--;
        }

        return -1;
    }
};

int main()
{
    Solution soln;
    vector<int> v = { 1, 7, 3, 6, 5, 6 };
    cout << soln.pivotIndex(v) << endl;
    return 0;
}
