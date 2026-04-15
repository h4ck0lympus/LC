#include "bits/stdc++.h"
using namespace std;
class Solution {
public:
    string longestPalindrome(string s) {
        int n = s.size();
        // dp[i][j] = is substring s[i,j] palindrome or not
        vector<vector<bool>> dp(n, vector<bool>(n, 0));
        int mx = 1;
        string res = s.substr(0, 1);
        for (int i=0; i < n; i++) dp[i][i] = 1;
        for (int len=2; len <= n; len++) {
            for (int l=0; l + len - 1 < n; l++) {
                int r = l + len - 1;
                if (s[l] == s[r]) {
                    if (len == 2) dp[l][r] = 1;
                    else if (l+1 <= r-1 && dp[l+1][r-1]) dp[l][r] = 1;
                } else {
                    dp[l][r] = 0;
                }
                if (dp[l][r] && r - l + 1 > mx)  {
                    mx = r - l + 1;
                    res = s.substr(l, r-l+1);
                }
            }
        }
        return res;
    }
};

int main() {
    Solution s = *new Solution();
    cout << s.longestPalindrome("babad") << "\n";
}
