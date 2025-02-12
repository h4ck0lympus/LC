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
        map<int, pair<int, int>> m;

        int res = -1;

        for (int num: nums) {
            int sum = sum_of_digits(num);
            // at max m[sum] will have size of 2
            // we will only keep max 2 elements encountered till now
            if (m[sum].first == 0) {
                m[sum].first = num;
            } else if (m[sum].second == 0) {
                m[sum].second = num;
            } else if (m[sum].first > m[sum].second){
                m[sum].second= num;
            } else {
                m[sum].first = num;
            }

            // if both pair are initialized
            if (m[sum].first && m[sum].second) {
                res = max(res, m[sum].first + m[sum].second);
            }
        }

        return res;
    }
};
