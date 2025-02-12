class Solution {
public:
    // [0,1,0,3,12]
    // [1,0,0,3,12]
    // [1,0,0,3,12] // swapped the zeros
    // [1,0,3,0,12]
    // [1,0,3,12,0]
    // [1,3,0,12,0]
    // [1,3,12,0,0]
    
    void moveZeroes(vector<int>& nums) {
        int n = nums.size();
        bool swapped = true;

        while (swapped) {
            swapped = false;
            for (int i=0; i < n-1; i++) {
                if (nums[i] == 0) {
                    int temp = nums[i+1];
                    nums[i+1] = nums[i];
                    nums[i] = temp;
                    swapped = true;
                }
            }
        }
    }
};
