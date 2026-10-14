class Solution {
public:
    string smallestString(string s) {       
        int n = s.size();
        int i=0;

        while (i < n && s[i] == 'a') i++; // skip all the a's we see from left
        
        bool changed = false;
        if (i < n && s[i] != 'a') {
            changed = true;
            while (i < n && s[i] != 'a') {
                s[i]--;
                i++;
            }
        }

        while (i < n && s[i] == 'a') i++; // skip all trailing a's

        if (!changed && i == n) {
            i--;
            if (s[i] == 'a') s[i] = 'z';
            else s[i]--; 
        }
        return s;
    }
};

// greedy
// first char smallest (a) -> swap substring from 1st pos to most recent seen a / till end
// swap till most recent seen a char
