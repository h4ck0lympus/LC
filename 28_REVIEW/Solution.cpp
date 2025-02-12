class Solution {
public:
    int strStr(string haystack, string needle) {
        int n = needle.size();

        vector<int> lps(n, 0); // LPS table for needle
        int prev_match = 0;

        // starting from index = 1, because 0 is not checked for prefix
        int i = 1;

        while (i < n) {
            if (needle[i] == needle[prev_match]) {
                prev_match++;
                lps[i] = prev_match;
                i++;
            } else {
                // if needle doesn't match
                if (prev_match > 0) {
                    // prev match more than 0, then check one character behind
                    prev_match = lps[prev_match - 1];
                } else {
                    lps[i] = 0;
                    i++;
                }
            }
        }

        // Using the LPS table and using it to find match
        i=0; // haystack pointer
        int j=0; // neeedle pointer

        while (i < haystack.size()) {
            if (haystack[i] == needle[j]) {
                i++;
                j++;
            } else {
                if (j > 0) {
                    j = lps[j - 1];
                } else {
                    i++;
                }
            }

            if (j == n) {
                return i - j;
            }
        }

        return -1;
    }
};
