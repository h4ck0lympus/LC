class Solution {
public:
    void cs2(int idx, int target, vector<int>& candidates, vector<vector<int>>& res, vector<int>& arr) {
        int N = candidates.size();

        // base case - if solution found
        if (idx <= N && target == 0) {
            res.push_back(arr);
            return;
        }

        // another case in case solution won't exist and we are still in that branch, quit it
        if (idx >= N || target < 0) {
            return;
        }

        // branch into possibility of choosing the number
        arr.push_back(candidates[idx]);
        cs2(idx + 1, target - candidates[idx], candidates, res, arr);

        // skip the dupes
        while (idx < N - 1 && candidates[idx] == candidates[idx + 1]) idx++;

        // branch into possibility of not choosing the number 
        arr.pop_back(); // remove the number which was added earlier
        cs2(idx + 1, target, candidates, res, arr);
    }

    vector<vector<int>> combinationSum2(vector<int>& candidates, int target) {
        vector<vector<int>> res;
        vector<int> arr;
        sort(candidates.begin(), candidates.end());
        cs2(0, target, candidates, res, arr);
        return res;
    }
};
