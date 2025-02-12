class Solution {
public:
    void cs(vector<int> candidates, vector<vector<int>>& res, vector<int>& arr, int target, int sum, int idx) {
        if (sum == target) {
            res.push_back(arr);
            return;
        } 

        if (idx >= candidates.size() || sum > target) {
            return;
        }

        // branch where we are considering
        arr.push_back(candidates[idx]);
        cs(candidates, res, arr, target, sum + candidates[idx], idx);
        arr.pop_back(); // not consider it
        cs(candidates, res, arr, target, sum, idx + 1); // move to next
    }

    vector<vector<int>> combinationSum(vector<int>& candidates, int target) {
        vector<vector<int>> res;
        vector<int> arr;
        cs(candidates, res, arr, target, 0, 0);
        return res;
    }
};

