class Solution {
public:

    bool isPalindrome(string s) {
        int i = 0;
        int j = s.size() - 1;

        bool res;

        while (i < j && (res = s[i] == s[j])) {
            i++; j--;
        }

        return res;
    }

    void p(int idx, string s, vector<vector<string>>& res, vector<string>& arr) {
        if (idx >= s.size()) {
            res.push_back(arr);
            return;
        }

        if (idx < s.size()) {
            string first_part = s.substr(idx, idx + 1);
            string second_part = s.substr(idx + 1, s.size());

            // branch into partitioning one if palindrome
            arr.push_back(s.substr(idx, idx + 1));
            if (isPalindrome(first_part)) {
                p(idx + 1, second_part, res, arr);
            } 

            arr.pop_back();
        } 

        if (idx < s.size() - 1) {
            string first_part = s.substr(idx, idx + 2);
            string second_part = s.substr(idx + 2, s.size());

            // branch into partitioning second
            arr.push_back(s.substr(idx, idx + 2));

            if (isPalindrome(first_part)) {
                p(idx + 2, second_part, res, arr);
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
