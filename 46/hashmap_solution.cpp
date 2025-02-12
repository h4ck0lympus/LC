class Solution {
public:
    map<int, int> m;

    void p(vector<int>& nums, vector<vector<int>>& res, vector<int>& arr) {
        int n = nums.size();

        if (arr.size() == n) {
            res.push_back(arr);
            return;
        }

        for (int i: nums) {
            if (!m[i]) {
                arr.push_back(i);
                m[i] = 1;
                p(nums, res, arr);
                m[i] = 0;
                arr.pop_back();
            }
        }
    } 

    vector<vector<int>> permute(vector<int>& nums) {
        vector<vector<int>> res;
        vector<int> arr;

        for (int i: nums) {
            m[i] = 0;
        }

        p(nums, res, arr);

        return res;
    }
};
