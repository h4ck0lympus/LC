#include <bits/stdc++.h>

using namespace std;

class Solution {
public:
    void setZeroes(vector<vector<int>>& matrix)
    {
        // record occuring of zero in first row and column
        // first row = matrix[0][]
        // first col = matrix[][0]

        int M = matrix.size();
        int N = matrix[0].size();

        int recCol = 1;

        // mark for whole matrix
        // first horizontal, account for verticals (columns)
        // first vertical, account for horizontals (rows)
        for (int i = 0; i < M; i++) {
            for (int j = 0; j < N; j++) {
                if (matrix[i][j] == 0) {
                    matrix[i][0] = 0;

                    if (j == 0) {
                        recCol = 0;
                    } else {
                        matrix[0][j] = 0;
                    }
                }
            }
        }

        // mark smaller matrix
        for (int row = 1; row < M; row++) {
            for (int col = 1; col < N; col++) {
                if (matrix[row][col]) {
                    bool check = (matrix[row][0] == 0 || matrix[0][col] == 0);
                    if (check) {
                        matrix[row][col] = 0;
                    }
                }
            }
        }

        // handle edge case
        if (matrix[0][0] == 0) {
            for (int col = 0; col < M; col++) {
                matrix[0][col] = 0;
            }
        }

        if (recCol == 0) {
            for (int row = 0; row < N; row++) {
                matrix[row][0] = 0;
            }
        }
    }
};
