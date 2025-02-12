/**
 * @author      : HackOlympus (zeus@hackolympus)
 * @file        : Solution
 * @created     : Sunday Mar 12, 2023 01:24:52 MST
 */

#include <bits/stdc++.h>

#define FOR(n) for (int i=0; i < n ; i++)

using namespace std ; 

class Solution {
public:
    vector<int> runningSum(vector<int>& nums) {
        vector<int> res = {nums[0]}; 

        int prev = 0; 

        for (int i=1; i < nums.size(); i++) {
            res.push_back(res[prev] + nums[i]);
            prev++;
        }

        return res;
    }
};

int main() 
{
    Solution soln; 
    vector<int> v = {1,2,3,4};
    vector<int> res = soln.runningSum(v);

    for (int e: res) {
        cout << e << " "; 
    }

    cout << endl;

    return 0;
}
