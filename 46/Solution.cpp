class Solution {
public:
    void p(int idx, vector<int>& nums, vector<vector<int>>& res, vector<int>& arr) {
        if (arr.size() == nums.size()) {
            res.push_back(arr);
            return;
        }

        for (int i=idx; i < nums.size(); i++) {
            int temp;
            arr.push_back(nums[i]);

            // swap
            temp = nums[i];
            nums[i] = nums[idx];
            nums[idx] = temp;

            p(idx+1, nums, res, arr);

            // unswap for next branch
            temp = nums[i];
            nums[i] = nums[idx];
            nums[idx] = temp;
            arr.pop_back();
        }
    } 

    vector<vector<int>> permute(vector<int>& nums) {
        vector<vector<int>> res;
        vector<int> arr;
        p(0, nums, res, arr);
        return res;
    }
};
