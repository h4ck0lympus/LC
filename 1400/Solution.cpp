class Solution {
public:
    bool canConstruct(string s, int k) {
        // not enough characters to make palindrome string
        if (s.size() < k) {
            return false;
        } 

        // each character can become one seperate string
        if (s.size() == k) {
            return true;
        }

        // calculate character
        int freq[26] = {0};
        for (char c: s) {
            freq[c - 'a']++;
        }

        // calculate odd frequency
        int oddfreq  = 0;

        // now we don't need to calculate even character because their existance doesn't effect 
        // creation of palindrome, what effect's is number of odd characters
        for (int f: freq) {
            if (f && f % 2 != 0) {
                oddfreq++;
            }
        }

        return oddfreq <= k;
    }
};
