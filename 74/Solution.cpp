#include <bits/stdc++.h>

using namespace std;

class Solution {
public:
    bool searchInCol(vector<int>& matrix, int target)
    {
        int start = 0;
        int end = matrix.size() - 1;

        while (start <= end) {
            int mid = end - start;
            if (matrix[mid] == target) {
                return true;
            } else if (target < matrix[mid]) {
                // go left
                end = mid - 1;
            } else {
                // go right
                start = mid + 1;
            }
        }
        return false;
    }

    bool searchMatrix(vector<vector<int>>& matrix, int target)
    {
        int rows = matrix.size();
        int cols = matrix[0].size();

        for (int i = 0; i < rows; i++) {
            vector<int> col = matrix[i];
            int start = col[0];
            int end = col[cols - 1];

            if (target == start || target == end) {
                return true;
            } else if (target > start && target < end) {
                return searchInCol(col, target);
            }
        }

        return false;
    }
};
