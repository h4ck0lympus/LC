class Solution {
public:
    int maxScore(string s) {
        int n = s.size();
        int rightSum = 0;
        int leftSum = 0;

        for (char c: s) {
            rightSum += (c == '1');
        }

        int res = 0;
        for (int i=0; i < n-1; i++) {
            leftSum += (s[i] == '0');
            rightSum -= (s[i] == '1');
            res = max(res, leftSum+rightSum);
        }
        return res;
    }
};
