class Solution {
public:
    int maximumSum(vector<int>& nums) {
        // 10^9 is maximum
        // max possible sum of digits = 9 + 9 + 9 + 9 + 9 + 9 + 9 + 9 + 9 = 81
        
        // m[i] will hold the maximum number encountered so far with sum of digits = i
        vector<int> m(82, -1);
        int res = -1;


        for (int num: nums) {
            // find sum of digits
            int sum = 0;
            int temp = num;
            while (temp) {
                sum += temp % 10;
                temp /= 10;
            }

            if (m[sum] != -1) { 
                // find the sum of current num with max number, store it in res
                res = max(res, num + m[sum]);
            }

            m[sum] = max(m[sum], num); // store the maximum number till now
        }

        return res;
    }
};
