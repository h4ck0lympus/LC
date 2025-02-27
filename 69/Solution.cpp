class Solution {
public:
    int mySqrt(int x) {
        int start = 0;
        int end = x;

        while (start <= end) {
            int mid = start + (end - start) / 2;
            long long val = 1L * mid * mid;
            if (val == (long long)x) {
                return mid;
            } else if (val < x) {
                start = mid + 1;
            } else {
                end = mid - 1;
            }
        }

        return end;
    }
};
