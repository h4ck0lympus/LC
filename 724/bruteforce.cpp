/**
 * @author      : HackOlympus (zeus@hackolympus)
 * @file        : Solution
 * @created     : Sunday Mar 12, 2023 01:48:08 MST
 */

#include <bits/stdc++.h>
#include <vector>

using namespace std ; 

class Solution {
public:
    int rightSum(vector<int>& nums, int ind) {
        int sum = 0; 
        
        for (int i=ind; i < nums.size(); i++) {
            sum += nums[i];
        }

        return sum;
    }
    
    int leftSum(vector<int>& nums, int ind) {
        int sum = 0; 

        for (int i=0; i <= ind; i++) {
            sum += nums[i]; 
        }

        return sum;
    }

    int pivotIndex(vector<int>& nums) {
        if (rightSum(nums, 1) == 0) {
            return 0;
        }

        for (int ind=0; ind < nums.size(); ind++) {
            if (rightSum(nums, ind) == leftSum(nums, ind)) {
                return ind;
            }
        }

        return -1;
    }
};

int main()
{
    Solution soln;
    vector<int> v = {2,1,-1};
    cout << soln.pivotIndex(v) << endl;
    return 0 ; 
}


