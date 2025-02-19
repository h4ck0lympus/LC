class Solution {
public:
    string reverseWords(string s) {
        stack<string> words;
        string word = "";
        int i = 0;
        while (i < s.size() && s[i] == ' ')
            i++;

        while (i < s.size()) {
            char ch = s[i];
            if (ch == ' ') {
                if (!word.empty())
                    words.push(word);
                word = "";
                i++;
                while (i < s.size() && s[i] == ' ')
                    i++;
            } else {
                word += ch;
                i++;
            }
        }
        if (!word.empty())
            words.push(word);

        string ans = "";
        while (words.size() > 1) {
            ans += words.top() + " ";
            words.pop();
        }
        ans += words.top(); words.pop();
        return ans;
    }
};
