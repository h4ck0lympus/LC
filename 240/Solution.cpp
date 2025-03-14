class Solution {
public:
    bool searchMatrix(vector<vector<int>>& matrix, int target) {
        // first find column
        int c1 = 0, c2 = matrix[0].size();
        
        while (c1 <= c2)  {
            int mid = (c1 + c2) / 2;

            int element = matrix[0][mid];

            if (element == target) {
                return true;
            } else if (element < target) {
                c1 = mid + 1;
            } else {
                c2 = mid - 1;
            }
        }

        int r1 = 0, r2 = matrix.size();

        while (r1 <= r2) {
            int mid = (r1 + r2) / 2;
            
            int e1 = matrix[mid][c1];

            if (e1 == target) {
                return true;
            } else if (e1 < target) {
                r1 = mid + 1;
            } else {
                r1 = mid - 1;
            }
        }

        r1 = 0, r2 = matrix.size();

        while (r1 <= r2) {
            int mid = (r1 + r2) / 2;
            
            int e1 = matrix[mid][c2];

            if (e1 == target) {
                return true;
            } else if (e1 < target) {
                r1 = mid + 1;
            } else {
                r1 = mid - 1;
            }
        }

        return false;
    }
};
