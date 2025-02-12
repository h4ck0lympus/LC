class Solution {
public:
    bool canBeValid(string s, string locked) {
        int n = s.size();
        if (n % 2 != 0) {
            return false;
        }

        stack<int> open;
        stack<int> flexible;

        for (int i = 0; i < n; i++) {
            if (locked[i] == '0') {
                flexible.push(i);
            } else if (s[i] == '(') {
                open.push(i);
            } else { 
                if (!open.empty()) {
                    open.pop(); 
                } else if (!flexible.empty()) {
                    flexible.pop(); 
                } else {
                    return false;
                }
            }
        }

        while (!open.empty() && !flexible.empty()) {
            if (flexible.top() > open.top()) {
                open.pop();
                flexible.pop();
            } else {
                break;
            }
        }

        return open.empty();
    }
};

