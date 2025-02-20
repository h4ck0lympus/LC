class Solution {
public:
    int findMin(vector<int>& nums) {
        int start=0, end = nums.size() - 1;
        int res = INT_MAX;

        while (start <= end) {
            int mid = start + (end - start) / 2;
            // find the sorted part
            if (nums[start] <= nums[mid]) {
                // choose the minimum in the sorted part
                res = min(res, nums[start]);
                // move to other part
                start = mid + 1;
            } else {
                res = min(res, nums[mid]);
                end = mid - 1;
            }
        }
        return res;
    }
};
