class Solution {
public:
    void sortColors(vector<int>& nums) {
        // one ptr basically acts as check ptr
        int zero_ptr = 0, one_ptr = 0, two_ptr = nums.size() - 1;

        while (one_ptr <= two_ptr) {
            switch(nums[one_ptr]) {
                case 0:
                    swap(nums[zero_ptr++], nums[one_ptr++]);
                    break;
                case 1:
                    // swap(nums[one_ptr], nums[one_ptr]);
                    one_ptr++;
                    break;
                case 2:
                    swap(nums[two_ptr--], nums[one_ptr]);
                    break;
                default:
                    break;
            }
        }
    }
};
