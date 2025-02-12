class Solution {
public:
    void swd(vector<int>& nums, vector<vector<int>>& res, vector<int>& arr, int idx) {
        if (idx >= nums.size()) {
            res.push_back(arr);
            return;
        } 

        // choose the number
        arr.push_back(nums[idx++]);
        while (idx < nums.size() && nums[idx] == nums[idx - 1])
            idx++;

        swd(nums, res, arr, idx);

        // not choose the number
        arr.pop_back();
        swd(nums, res, arr, idx);

        return;
    }

    vector<vector<int>> subsetsWithDup(vector<int>& nums) {
        int idx = 0;
        vector<vector<int>> res;
        vector<int> arr;

        swd(nums, res, arr, idx);
        
        return res;
    }
};
