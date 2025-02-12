class Solution {
public:
    int sum_of_digits(int num) {
        int res = 0;
        while (num) {
            res += num % 10;
            num /= 10;
        }
        return res;
    }

    int maximumSum(vector<int>& nums) {
        // sum: nums with that sum
        map<int, vector<int>> m;

        int res = -1;

        for (int num: nums) {
            int sum = sum_of_digits(num);
            m[sum].push_back(num);
            if (m[sum].size() > 1)  {
                sort(m[sum].begin(), m[sum].end(), [](int a, int b){return a > b;}); // to be changed to some kind of DS that automatically keeps number in sorted manner
                int s = m[sum][0] + m[sum][1];
                res = max(res, s);
            }
        }

        return res;
    }
};
