class Solution {
public:
    // s -> og string
    // p -> pattern string
    bool isMatch(string s, string p) {
        int n = s.size(), m = p.size();
        // dp[i][j] = if s[0..i-1] matches p[0..j-1]
        vector<vector<uint8_t>> dp(n+1, vector<uint8_t>(m+1, 0));
        dp[0][0] = true; // empty string matches another empty string
        
        // dp[0][j] = true if first char in string is empty and p[j] == '*'
        for (int j=1; j <= m; j++) {
            if (p[j-1] == '*') {
                dp[0][j] = dp[0][j-1]; // empty string matches "*"
            } 
        }

        for (int i=1; i <= n; i++) {
            for (int j=1; j <= m; j++) {
                if (p[j-1] == '*') { // either consume the * character or not consume it (for next char in string)
                    dp[i][j] = dp[i-1][j] || dp[i][j-1];
                } else if (p[j-1] == '?') {
                    dp[i][j] = dp[i-1][j-1]; // consume both character
                } else if (s[i-1] == p[j-1]) {
                    dp[i][j] = dp[i-1][j-1];
                }
            }
        }

        return dp[n][m];
    }
};


// first thought: we can do this using some sort of SM architecture
// parse the pattern and make SM out of it
// run the string on the SM and run output
