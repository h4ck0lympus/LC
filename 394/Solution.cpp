#include "bits/stdc++.h"
using namespace std;
class Solution {
public:
    string decodeString(string s) {
        stack<pair<string, int>> chars;
        int num = 0;
        string curr_str = "";

        for (char ch: s) {
            if (isdigit(ch)) {
                num = (num * 10) + (ch - '0');
            } else if (ch == '[') {
                chars.push({curr_str, num});
                curr_str = "";
                num =0;
            } else if (ch == ']') {
                auto [prev_str, prev_num] = chars.top(); chars.pop();
                string str = "";
                for (int i=0; i < prev_num; i++) {
                    str += curr_str;
                }
                curr_str = prev_str + str;
            } else {
                curr_str += ch;
            }
        }
        return curr_str;
    }
};

// first intutition: 
// keep a stack of number and characters
// whenever closing bracket, pop from number stack 
// pop from character stack 
// 
