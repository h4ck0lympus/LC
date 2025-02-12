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

        for (int i=idx; i < N; i++) {
            if (i > idx && candidates[i-1] == candidates[i]) continue;

            // choose the number
            arr.push_back(candidates[i]);
            cs2(i + 1, target - candidates[i], candidates, res, arr);
            
            // for next recursion call we won't choose this number
            arr.pop_back();
        }

    }

    vector<vector<int>> combinationSum2(vector<int>& candidates, int target) {
        vector<vector<int>> res;
        vector<int> arr;
        sort(candidates.begin(), candidates.end());
        cs2(0, target, candidates, res, arr);
        return res;
    }
};
