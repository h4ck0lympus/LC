class Solution {
public:
    string clearDigits(string s) {
        vector<char> stack;

        for (int i=0; i < s.size(); i++)  {
            char ch = tolower(s[i]);
            if ('a' <= ch && ch <= 'z') {
                stack.push_back(ch);
            } else {
                stack.pop_back();
            }
        }

        return string(stack.begin(), stack.end());
    }
};
