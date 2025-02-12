class Solution {
public:
    int longestMonotonicSubarray(vector<int>& nums) {
        // [3, 2, 1]
        int max_count = 1;

        int inc_count = 1;
        int dec_count = 1;

        int last_occ = nums[0];

        // check for increasing and decreasing in same loop order
        for (int i=1; i < nums.size(); i++) {
            if (nums[i] > last_occ) {
                inc_count++;
                dec_count = 1;
            } else if (nums[i] < last_occ) {
                dec_count++;
                inc_count = 1;
            } else {
                inc_count = 1;
                dec_count = 1;
            }

            max_count = max(max_count, max(inc_count, dec_count));
            last_occ = nums[i];
        }

        return max_count;
    }
};
