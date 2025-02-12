class Solution {
public:
    void cs(vector<int> candidates, vector<vector<int>>& res, vector<int>& arr, int target, int idx) {
        if (idx <= candidates.size() && target == 0) {
            res.push_back(arr);
            return;
        } 

        if (idx >= candidates.size() && target < 0) {
            return;
        }

        // branch where we are considering
        if (candidates[idx] <= target) {
            arr.push_back(candidates[idx]);
            cs(candidates, res, arr, target - candidates[idx], idx);
            arr.pop_back(); // not consider it
        }

        cs(candidates, res, arr, target, idx + 1); // move to next

        return; // if no result found
    }

    vector<vector<int>> combinationSum(vector<int>& candidates, int target) {
        vector<vector<int>> res;
        vector<int> arr;
        cs(candidates, res, arr, target, 0);
        return res;
    }
};

