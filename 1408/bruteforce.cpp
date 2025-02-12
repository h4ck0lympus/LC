class Solution {
public:
    vector<string> stringMatching(vector<string>& words) {
        sort(words.begin(), words.end(), [](string& a, string& b){return a.length() > b.length();});
        vector<string> res;
        unordered_set<string> seen;

        for (int i=0; i < words.size(); i++) {
            for (int j=i+1; j < words.size(); j++) {
                if (words[i].find(words[j]) != -1 && seen.find(words[j]) == seen.end()) {
                    res.push_back(words[j]);
                    seen.insert(words[j]);
                }
            }
        }

        return res;
    }
};
