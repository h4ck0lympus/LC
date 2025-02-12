//time complexity: O(n log n) + O(2^n) + set operation

class Solution {
public:
    vector<vector<int>> subsetsWithDup(vector<int>& nums) {
        sort(nums.begin(), nums.end());
        set<vector<int>> res;
        int n = nums.size();
        int totalSubsets = 1 << n; // 2^n
                             
        for (int mask=0; mask < totalSubsets; mask++) {
            vector<int> curr;
            for (int i=0; i < n; i++) {
                if (mask & (1 << i)) {
                    curr.push_back(nums[i]);
                }
            }

            res.insert(curr);
        }

        return vector<vector<int>>(res.begin(), res.end());
    }
};
