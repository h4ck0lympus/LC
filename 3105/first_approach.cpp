class Solution {
public:
    int longestMonotonicSubarray(vector<int>& nums) {
        // [3, 2, 1]
        int max_count = 1;

        int curr_count = 1;
        int last_occ = nums[0];

        // check for increasing order
        for (int i=1; i < nums.size(); i++) {
            if (nums[i] > last_occ) {
                curr_count++;
            } else {
                curr_count = 1;
            }

            max_count = max(max_count, curr_count);

            last_occ = nums[i];
        }

        last_occ = nums[0];
        curr_count = 1;

        // check for decreasing order
        for (int i=1; i < nums.size(); i++) {
            max_count = max(max_count, curr_count);

            if (nums[i] < last_occ) {
                curr_count++;
            } else {
                curr_count = 1;
            }
            max_count = max(max_count, curr_count);

            last_occ = nums[i];
        }

        return max_count;
    }
};
