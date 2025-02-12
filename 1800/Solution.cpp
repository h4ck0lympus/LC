class Solution {
public:
    int maxAscendingSum(vector<int>& nums) {
        int max_sum = nums[0];
        int curr_sum = nums[0];
        int last_num = nums[0];

        // [10,20,30,5,10,50]
        for (int i=1; i < nums.size(); i++) {
            if (nums[i] > last_num) {
                curr_sum = curr_sum + nums[i];
            } else {
                curr_sum = nums[i];
            }
            max_sum = max(curr_sum, max_sum);
            last_num = nums[i];
        }

        return max_sum;
    }
};
