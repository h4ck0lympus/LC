class Solution {
public:
    // recursive binary search
    int bs(vector<int>& nums, int start, int end, int target) {
        if (start > end) {
            return -1;
        }

        int mid = start + (end - start) / 2;

        if (nums[mid] == target) {
            return mid;
        } else if (nums[mid] < target) {
            return bs(nums, mid+1, end, target);
        } else {
            return bs(nums, start, mid-1, target);
        }
    }

    vector<int> searchRange(vector<int>& nums, int target) {
        int left = -1;
        int right = -1;
        
        int start = 0;
        int end = nums.size() - 1;

        while (true) {
            int idx = bs(nums, start, end, target);

            if (idx == -1) {
                break;
            }

            left = idx;
            end = idx - 1;
        }

        if (left == -1)
            return {-1, -1};

        start = left;
        end = nums.size() - 1;

        while (true) {
            int idx = bs(nums, start, end, target);
            if (idx == -1) {
                break;
            }

            right = idx;
            start = idx + 1;
        }

        return {left, right};
    }
};
