class Solution {
public:
    vector<int> rearrangeArray(vector<int>& nums) {
        vector<int> pos;
        vector<int> neg;
        for (int num: nums) {
            if (num >= 0) {
                pos.push_back(num);
            } else {
                neg.push_back(num);
            }
        }

        int p = 0;
        int n = 0;

        for (int i=0; i < nums.size(); i++) {
            if (i & 1 == 1) {
                // odd
                nums[i] = neg[n++];
            } else {
                nums[i] = pos[p++];
            }
        }
        
        return nums;
    }
};
