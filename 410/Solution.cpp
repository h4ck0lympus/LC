class Solution {
public:
    // number of subarrays possible
    int subArraysPossible(vector<int>& nums, int sumPossible) {
        int count = 1; // atleast one subarray is possible, the entire array
        int cs = 0;  // current sum
        for (int i=0; i < nums.size(); i++) {
            if (cs + nums[i] <= sumPossible) {
                cs += nums[i];
            } else {
                count++;
                cs = nums[i];
            }
        }

        return count;
    }

    int splitArray(vector<int>& nums, int k) {
        int low = *max_element(nums.begin(), nums.end());
        int high = accumulate(nums.begin(), nums.end(), 0);

        while (low <= high) {
            int sumOfSub = low + (high - low) / 2;

            if (subArraysPossible(nums, sumOfSub) == k) {
                // we need to minimize so move left
                high = sumOfSub - 1;
            }  else {
                low = sumOfSub + 1;
            }
        }

        return low;
    }
};
