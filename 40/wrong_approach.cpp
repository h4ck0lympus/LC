#include <bits/stdc++.h>

using namespace std;

class Solution {
public:
    void cs(vector<int>& candidates, vector<vector<int>>& res, vector<int>& arr, int target, int idx) {
        if (idx <= candidates.size() && target == 0) {
            res.push_back(arr);
            return;
        }

        if (idx > candidates.size() || target < 0) {
            return;
        }

        // checking if still in bounds and not taking any dupes
        if (candidates[idx] <= target) {
            // take the number branch
            arr.push_back(candidates[idx]);
            cs(candidates, res, arr, target - candidates[idx], idx + 1); // +1 because we cannot have the dupes
            arr.pop_back();
        }
        cs(candidates, res, arr, target, idx + 1); // +1 because we move to next idx
    }

    vector<vector<int>> combinationSum2(vector<int>& candidates, int target) {
        sort(candidates.begin(), candidates.end()); // sort so we can differentiate between duplicates
        vector<vector<int>> res;
        vector<int> arr;
        cs(candidates, res, arr, target, 0);
        return res;
    }
};

int main (int argc, char *argv[]) {
    Solution s = *new Solution();
    vector<int> candidates = {10,1,2,7,6,1,5};
    int target = 8;
    s.combinationSum2(candidates, target);
    return 0;
}
