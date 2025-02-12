#include <bits/stdc++.h>

using namespace std;

// THIS IS SO STUPID

class Solution {
public:
    vector<vector<bool>> dp; 

    void p(int idx, string s, vector<vector<string>>& res, vector<string>& arr) {
        // aab
        int n = s.size(); // 3
        if (idx == n) {
            res.push_back(arr);
            return;
        }

        for (int i=idx; i < n; i++) { 
            // paritions: |aab, a|ab, aa|b, aab|
            //            "", "ab", "b", "aab"
            
            if (dp[idx][i]) {
                string substring = s.substr(idx, i - idx + 1); // +1 because substr takes length
                arr.push_back(substring);
                p(i + 1, s, res, arr); // "aab", "ab", "b", "", start from i + 1 substring
                arr.pop_back();
            }
        }
        
    }

    vector<vector<string>> partition(string s) {
        int n = s.size();
        vector<vector<string>> res;
        vector<string> arr;

        // dp [start][end]
        dp = vector<vector<bool>>(n, vector<bool>(n, false));

        // for all single chars in string
        for (int i=0; i < n; i++) {
            dp[i][i] = true;
        }

        // for all double chars in string 
        for (int i = 0; i < n - 1; i++) {
            dp[i][i+1] = (s[i] == s[i+1]);
        }

        // length >= 3
        for (int length = 3; length <= n; length++) {
            for (int start = 0; start + length - 1 < n; start++) {
                int end = start + length - 1;
                if (s[start] == s[end] && dp[start + 1][end - 1]) {
                    dp[start][end] = true;
                }
            }
        }

        p(0, s, res, arr);
        return res;
    }
};
