class Solution {
public:
    int smallestDivisor(vector<int>& nums, int threshold) {
        int low = 1, high = INT_MIN;

        for (int num: nums)
            high = (high < num) ? num: high;
        
        int ans = 0;

        while (low <= high) {
            int mid = low + (high - low) / 2;
            int divisor_sum = 0;

            for (int num: nums) {
                if (num % mid == 0)
                    divisor_sum += num / mid;
                else
                    divisor_sum += (num / mid) + 1;
            }

            if (divisor_sum(nums, mid) <= threshold) {
                ans = mid;
                high = mid - 1; // look more towards the right because we need lowest
            } else {
                low = mid + 1;
            }
        }
        return ans;
    }
};
