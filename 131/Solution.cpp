class Solution {
public:
    // O(n / 2)
    bool isPalindrome(string s) {
        int i = 0, j = s.size() - 1;
        while (i < j) {
            if (s[i++] != s[j--])
                return false;
        }
        return true;
    }

    // O(2^n)
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
            string substring = s.substr(idx, i - idx + 1); // +1 because substr takes length
            
            if (isPalindrome(substring)) {
                arr.push_back(substring);
                p(i + 1, s, res, arr); // "aab", "ab", "b", "", start from i + 1 substring
                arr.pop_back();
            }
        }
        
    }


    vector<vector<string>> partition(string s) {
        vector<vector<string>> res;
        vector<string> arr;
        p(0, s, res, arr);
        return res;
    }
};
