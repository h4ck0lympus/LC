class Solution {
public:
    void p(int idx, int& k, vector<char>& nums, string& res, string& cstr)  {
        if (k == 0) {
            return;
        }

        if (nums.size() == cstr.size()) {
            res = cstr;
            k--;
            if (k == 0)
                return;
        }

        for (int i=idx; i < nums.size(); i++) {
            swap(nums[idx], nums[i]);
            cstr.push_back(nums[i]);

            p(idx + 1, k, nums, res, cstr);

            cstr.pop_back();
            swap(nums[idx], nums[i]);
            if (k == 0) return;
        }
    }

    string getPermutation(int n, int k) {
        char numbers[] = "123456789";
        vector<char> nums;
        for (int i=0; i < n; i++) {
            nums.push_back(numbers[i]);
        }
        string res, cstr;
        p(0, k, nums, res, cstr);
        return res;
    }
};
