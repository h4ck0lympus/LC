class Solution {
public:
    int search(vector<int>& nums, int target) {
        // first find the minimum element, that will help us get the shift
        // from there we can choose if we want to look at the right side or left side
        int n = nums.size();

        int start = 0, end = n-1;

        while (start <= end) {
            int mid = start + (end - start) / 2;
            if (nums[mid] == target) {
                return mid;
            } else if (nums[start] <= nums[mid]) { // first we identify the sorted part
                // left part is sorted
                if (nums[start] <= target && target <= nums[mid]) { // if lies in sorted part
                    // look into left part
                    end = mid - 1;
                } else {
                    // lies in unsorted part
                    start = mid + 1;
                }
            } else if (nums[mid] <= nums[end]) { 
                // right part is sorted
                if (target <= nums[end] && nums[mid] <= target) {
                    start = mid + 1;
                } else {
                    end = mid - 1;
                }
            }
        }
        return -1;
    }
};
