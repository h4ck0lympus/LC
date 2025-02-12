class Solution {
public:
    void swd(int idx, vector<int>& nums, vector<int>& ds, vector<vector<int>>& res) {
        res.push_back(ds);

        for (int i=idx; i < nums.size(); i++) {
            if (i != idx && nums[i-1] == nums[i]) continue;

            ds.push_back(nums[i]);
            swd(i, nums, ds, res);
            ds.pop_back();
        }

        return;
    }

    vector<vector<int>> subsetsWithDup(vector<int>& nums) {
        // so that all the dupes are together
        sort(nums.begin(), nums.end());

        vector<vector<int>> res;
        vector<int> ds;

        swd(0, nums, ds, res);
        
        return res;
    }
};

