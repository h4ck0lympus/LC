class Solution {
public:
    int myAtoi(string s) {
        int N = s.size();
        bool neg = false;
        int ans = 0;
        int idx = 0;

        // ignore leading whitespace
        while (idx < N && (s[idx] == ' ' || s[idx] == '\t')) {
            idx++;
        }

        // check sign
        if (idx < N && (s[idx] == '-' || s[idx] == '+')) {
            neg = (s[idx] == '-');
            idx++;
        }

        // ignore trailing zeros
        while (idx < N && s[idx] == '0') {
            idx++;
        }

        while (idx < N && ('0' <= s[idx] && s[idx] <= '9')) {
            int num = s[idx] - '0';

            // if answer gets integer overflow
            if (ans > (INT_MAX - num) / 10) {
                return neg ? INT_MIN : INT_MAX;
            }

            ans = ans * 10 + num;
            idx++;
        }
        if (neg)
            ans = -ans;
        return ans;
    }
};
