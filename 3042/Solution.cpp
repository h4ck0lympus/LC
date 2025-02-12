class Solution {
public:
    bool isPrefixAndSuffix(string str1, string str2) {
        // str1 is both prefix and suffix of str2
        int l1 = str1.size();
        int l2 = str2.size();
        if (l1 > l2) {
            return false;
        }
        for (int i=0, j=l2-l1; i < l1 && j < l2; i++, j++) {
            if (str1[i] != str2[i] || str1[i] != str2[j]) {
                return false;
            }
        }
        return true;
    }

    int countPrefixSuffixPairs(vector<string>& words) {
        int count = 0;
        for (int i=0; i < words.size(); i++) {
            for (int j=i+1; j < words.size(); j++) {
                count += (isPrefixAndSuffix(words[i], words[j]) == true);
            }
        }
        
        return count;
    }
};
