class Solution {
public:
    int removeDuplicates(vector<int>& nums) {
        int length = nums.size();

        if (length <= 1) {
            return 1;
        }

        // 1 1 2
        int checkPtr = 0;
        int writePtr = 1;

        int lastCheckedNum = nums[0];

        while (checkPtr < length) {
            if (nums[checkPtr] != lastCheckedNum) {
                nums[writePtr] = nums[checkPtr];
                lastCheckedNum = nums[checkPtr];
                writePtr++;
            }
            checkPtr++;
        }
        return writePtr;
    }
};
