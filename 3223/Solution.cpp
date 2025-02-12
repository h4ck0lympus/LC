class Solution {
public:
    int minimumLength(string s) {
        int left = 0;
        int right = s.size() - 1;

        string newString = "";
        while (left != right) {
            if (s[left] == s[right]) {
                // if found 2 boundary left and right same char
                // find middle char
                // if middle char found, delete left and right
                for (int i=left; i < right; i++) {
                    if (s[left])
                }
            } else {
                left++;
                right--;
            }
        }
    }
};
