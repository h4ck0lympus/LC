class Solution {
public:
    int prefixCount(vector<string>& words, string pref) {
        int count = 0;
        for (string word: words) {
            bool flag = (pref[0] == word[0]);
            for (int i=1; flag && i < pref.length(); i++) {
                flag = (pref[i] == word[i]);
            }
            count += (flag == true);
        }
        return count;
    }
};
