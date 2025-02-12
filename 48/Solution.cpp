#include <bits/stdc++.h>

// constraints: it's always gonna be a square matrix

using namespace std;

class Solution {
public:
    void rotate(vector<vector<int>>& matrix)
    {
        int rows = matrix.size();
        int cols = matrix[0].size();
        // take transpose

        // outer loop: rows times
        for (int i = 0; i < rows; i++) {
            // inner loop is running i times
            // worst case it's gonna run for rows times
            // so O(n**2)
            for (int j = 0; j < i; j++) {
                int temp = matrix[i][j];
                matrix[i][j] = matrix[j][i];
                matrix[j][i] = temp;
            }
        }

        // reverse column wise
        // manually reversing :
        /*
        for (int i = 0; i < rows; i++) {
            int first = 0;
            int last = cols - 1;
            while (first <= last) {
                int temp = matrix[i][first];
                matrix[i][first] = matrix[i][last];
                matrix[i][last] = temp;
                first++;
                last--;
            }
        }
        */

        // faster way to reverse
        // using library
        for (int i = 0; i < rows; i++) {
            reverse(matrix[i].begin(), matrix[i].end());
        }
    }
};

int main(int argc, char* argv[])
{

    return 0;
}
