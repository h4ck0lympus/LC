class Solution {
public:
    int missingNumber(vector<int>& nums) {
        // xor1 = 1 ^ 2 ^ .... ^ n
        // xor2 = arr[0] ^ arr[1] ^ .... ^ arr[n]
        // xor1 ^ xor2 (1 ^ 1) ^ (2 ^ 2) ^ missing ^ (n ^ n) = missing
        // because x ^ x = 0
        int xor1 = 0, xor2 = 0;
        for (int i=0; i <=  nums.size(); i++) xor1 ^= i;
        for (int num: nums) xor2 ^= num;
        return xor1 ^ xor2;
    }
};
