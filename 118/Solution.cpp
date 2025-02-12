// observations :
// 1 2 3 4 ... pattern for length of sub arr
// rule 1 = start with 1
// rule 2 = edges of sub array = 1
// rule 3 =

// first solve then optimize

#include <bits/stdc++.h>

using namespace std;

class Solution {
public:
    vector<vector<int>> generate(int numRows)
    {
        vector<vector<int>> res;

        res.push_back({ 1 });

        if (numRows == 1)
            return res;

        res.push_back({ 1, 1 });

        if (numRows == 2)
            return res;

        for (int i = 2; i < numRows; i++) {
            vector<int> temp;
            vector<int> prev = res[i - 1];

            temp.push_back(1);

            for (int j = 1; j < i; j++) {
                int n = prev[j - 1] + prev[j];
                temp.push_back(n);
            }
            temp.push_back(1);
            res.push_back(temp);
        }

        return res;
    }
};
