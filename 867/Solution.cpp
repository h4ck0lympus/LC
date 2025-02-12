#include <bits/stdc++.h>

using namespace std;

class Solution {
public:
    vector<vector<int>> transpose(vector<vector<int>>& matrix)
    {
        int rows = matrix.size();
        int cols = matrix[0].size();

        if (rows == cols) {
            for (int i = 0; i < cols; i++) {
                for (int j = 0; j < i; j++) {
                    int temp = matrix[i][j];
                    matrix[i][j] = matrix[j][i];
                    matrix[j][i] = temp;
                }
            }
            return matrix;
        }

        vector<vector<int>> newMatrix;

        for (int i = 0; i < cols; i++) {
            vector<int> column;
            for (int j = 0; j < rows; j++) {
                column.push_back(matrix[j][i]);
            }
            newMatrix.push_back(column);
        }

        return newMatrix;
    }
};

int main(int argc, char* argv[])
{
    Solution s = *new Solution();
    vector<vector<int>> v = { { 1, 2, 3 },
        { 4, 5, 6 },
        { 7, 8, 9 } };
    s.transpose(v);
    return 0;
}
