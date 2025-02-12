// time complexity: 
// O(n * n)

class Solution {
public:
    int maxScore(string s) {
        int n = s.length();
        int score = 0;
        for (int i=1; i < n; i++) {
            string left = s.substr(0, i);
            string right = s.substr(i, n);

            int leftScore = 0;
            int rightScore = 0;

            for (char c: left) {
                if (c == '0') {
                    leftScore++;
                }
            }

            for (char c: right) {
                if (c == '1') {
                    rightScore++;
                }
            }

            if (leftScore + rightScore > score) {
                score = leftScore + rightScore;
            }
        }

        return score;
    }
};
