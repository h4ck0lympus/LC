class Solution {
public:
    bool canBeValid(string s, string locked) {
        int n = s.size();
        if (n % 2) {
            return false;
        }

        stack<int> st;
        st.push(0);
        for (int i=1; i < n; i++) {
            char top_char = s[st.top()];
            char curr_char = s[i];
            if (top_char == curr_char) {
                if (locked[i] == '0') {
                    st.pop();
                } else {
                    st.push(curr_char);
                }
            } else {
                if (top_char == '(' && curr_char == ')') {
                    st.pop();
                } else {
                    if (locked[i] == '0') {
                        st.pop();
                    } else {
                        st.push(curr_char);
                    }
                }
            }
        }

        return st.size() == 0;
    }
};
