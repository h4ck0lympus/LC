class Solution {
public:
    int get_value(char c) {
        switch(c) {
            case 'I': return 1;
            case 'V': return 5;
            case 'X': return 10;
            case 'L': return 50;
            case 'C': return 100;
            case 'D': return 500;
            case 'M': return 1000;
        }
        return -1;
    }
    int romanToInt(string s) {
        int num = 0;
        char prev = ' ';

        int i = 0;
        while (i < s.size()) {
            char curr = s[i];

            if (prev == 'I' && (curr == 'V' || curr == 'X')) {
                num -= get_value(prev);
                num += get_value(curr) - get_value(prev);
            } else if (prev == 'X' && (curr == 'L' || curr == 'C')) {
                num -= get_value(prev);
                num += get_value(curr) - get_value(prev);
            } else if (prev == 'C' && (curr == 'D' || curr == 'M')) {
                num -= get_value(prev);
                num += get_value(curr) - get_value(prev);
            } else {
                num += get_value(curr);
            }

            prev = curr;
            i++;
        }

        return num;
    }
};
