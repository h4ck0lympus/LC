class Solution {
public:
    // [3,4,5,1,2]
    bool check(vector<int>& nums) {
        int N = nums.size();
        int smallest_idx = 0;
        int smallest = nums[0];

        for (int i=1; i < N; i++) {
            if (nums[i] < smallest) {
                smallest = nums[i];
                smallest_idx = i;
            }
        }

        for (int i=smallest_idx; i < N - 1; i++) {
            // next number should be less than or equal to but not more than
            if (nums[i] > nums[i+1]) {
                return false;
            }
        }

        for (int i=0; i < smallest_idx; i++) {
            if (nums[i] > nums[i+1]) {
                return false;
            }
        }

        return true;
    }
};
