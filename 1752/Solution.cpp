class Solution {
public:
    // [3,4,5,1,2]
    bool check(vector<int>& nums) {
        int N = nums.size();
        int count = 0;

        for (int i=0; i < N-1; i++) {
            if (nums[i] > nums[i+1]) {
                count++;
            }
        }

        if (nums[0] < nums[N - 1]) {
            count++;
        }

        return count <= 1;
    }
};
