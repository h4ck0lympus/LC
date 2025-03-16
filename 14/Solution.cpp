class Solution {
public:
    string longestCommonPrefix(vector<string>& strs) {
        string prefix = strs[0];
        for (int i=0; i < strs.size(); i++) {
            int count = 0;
            int size = min(prefix.size(), strs[i].size());
            while (count < size && strs[i][count] == prefix[count]) count++;
            prefix = strs[i].substr(0, count);
        }
        return prefix;
    }
};
