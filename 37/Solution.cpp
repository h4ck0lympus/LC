#include <bits/stdc++.h>

using namespace std;

class Solution {
public:
    const int ROWS = 9;
    const int COLS = 9;

    bool validateRows(vector<vector<char>>& board, int row, int val) {
        for (int col=0; col < COLS; col++) {
            if ((board[row][col] != '.') && (board[row][col] - '0') == val) {
                return false;
            }
        }
        return true;
    }

    bool validateCols(vector<vector<char>>& board, int col, int val) {
        for (int row=0; row < ROWS; row++) {
            if ((board[row][col] != '.') && (board[row][col] - '0') == val) {
                return false;
            }
        }
        return true;
    }

    bool validateBox(vector<vector<char>>& board, int row, int col, int val) {
        int rowStart = row - (row % 3);
        int colStart = col - (col % 3);

        for (int row = rowStart; row < rowStart + 3; row++) {
            for (int col = colStart; col < colStart + 3; col++) {
                if (board[row][col] != '.' && val == (board[row][col] - '0')) {
                    return false;
                }
            }
        }

        return true;
    }

    bool validatePlacement(vector<vector<char>>& board, int row, int col, int val) {
        return validateRows(board, row, val) && validateCols(board, col, val) && validateBox(board, row, col, val);
    }

    bool solve(vector<vector<char>>& board, int row, int col) {
        if (row == ROWS) {
            return true;
        }

        if (col == COLS) {
            return solve(board, row+1, 0);
        }

        if (board[row][col] != '.') {
            return solve(board, row, col + 1);
        }

        for (int val = 1; val <= 9; val++) {
            if (validatePlacement(board, row, col, val)) {
                board[row][col] = (char)(val + '0');
                if (solve(board, row, col + 1)) {
                    return true;
                }
                board[row][col] = '.'; // change it back to empty cell
            }
        }

        return false;
    }

    void solveSudoku(vector<vector<char>>& board) {
        solve(board, 0, 0);
    }
};
