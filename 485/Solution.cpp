class Solution {
public:
    int findMaxConsecutiveOnes(vector<int>& nums) {
        int gMax = 0;
        int cMax = 0;
        for (int num: nums) {
            if (num) {
                cMax++;
            } else {
                cMax = 0;
            }
            gMax = max(gMax, cMax);
        }
        return gMax;
    }
};
