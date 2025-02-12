class Solution {
public:
    vector<vector<int>> threeSum(vector<int>& nums) {
        vector<vector<int>> res;
        sort(nums.begin(), nums.end());

        if (nums.size() < 3) {
            return {};
        }

        for (int low = 0; low < nums.size() - 2; low++) {
            // skip dupes
            if (low > 0 && nums[low] == nums[low-1]) {
                continue;
            }
            int target = nums[low];
            int first = low + 1;
            int second = nums.size() - 1;

            while (first < second) {
                int s = target + nums[first] + nums[second];

                if (s == 0) {
                    res.push_back({target, nums[first], nums[second]});

                    // skip dupes
                    while (first < second && nums[first] == nums[first + 1]) 
                        first++;

                    while (second > first && nums[second] == nums[second - 1]) 
                        second--;

                    first++;
                    second--;
                } else if (s < 0) {
                    first++;
                } else if (s > 0) {
                    second--;
                }
            }
        }
        return res;
    }
};
