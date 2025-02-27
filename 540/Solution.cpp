// TC: O(log n)
class Solution {
public:
    int singleNonDuplicate(vector<int>& nums) {
        // if only one element, it will be unique
        if (nums.size() == 1) 
            return nums[0];

        int ans = 0;
        int start = 0, end = nums.size() - 1;

        // if it's the first element
        if (nums[start] != nums[start + 1])
            return nums[0];

        // last element
        if (nums[end] != nums[end - 1])
            return nums[end];
        
        while (start <= end) {
            int mid = start + (end - start) / 2;

            // if it's the mid number
            if ((nums[mid] != nums[mid - 1]) && (nums[mid] != nums[mid + 1]))
                return nums[mid];
            else if ((mid % 2 == 0) && (nums[mid] == nums[mid + 1]))
                start = mid + 1;
            else if ((mid % 2 == 1) && (nums[mid] == nums[mid - 1]))
                start = mid + 1;
            else
                end = mid - 1;
        }

        return -1;
    }
};
