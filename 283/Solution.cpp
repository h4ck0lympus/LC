class Solution {
public:
    // [0,1,0,3,12]
    void moveZeroes(vector<int>& nums) {
        int n = nums.size();
        int write = 0;

        for (int i=0; i < n; i++) {
            if (nums[i] != 0) {
                int temp = nums[write];
                nums[write] = nums[i];
                nums[i] = temp;
                write++;
            }
        }
    }
};
