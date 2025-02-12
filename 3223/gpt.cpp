class Solution {
public:
    int minimumLength(string s) {
        vector<int> freq(256, 0);
        for (char c : s) {
            freq[c]++;
        }
        int answer = 0;
        for (int count : freq) {
            if (count > 0) {
                if (count % 2 == 1) {
                    answer += 1;
                } else {
                    answer += 2;
                }
            }
        }
        return answer;
    }
};

