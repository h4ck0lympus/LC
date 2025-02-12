class Solution {
public:
    // [0,1,0,3,12]
    // start check and write ptr from end
    void moveZeroes(vector<int>& nums) {
        int n = nums.size();
        int write = n - 1, check = n - 1;

        while (check >= 0)  {
            if (nums[check] == 0) {
                int temp = nums[write];
                nums[write] = nums[check];
                nums[check] = temp;
                write--;
            } else if (nums[check] < nums[write]) {
                int temp = nums[write];
                nums[write] = nums[check];
                nums[check] = temp;
            }
            check--;
        }
    }
};
