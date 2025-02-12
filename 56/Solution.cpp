#include <bits/stdc++.h>

using namespace std;

class Solution {
public:
    vector<vector<int>> merge(vector<vector<int>>& intervals)
    {
        // we will have to sort the intervals
        // we can't do without that

        int numIntervals = intervals.size();
        sort(intervals.begin(), intervals.end());

        int N = 0;
        int start = intervals[0][0];
        int finish = intervals[0][1];

        vector<vector<int>> res = { { start, finish } };

        for (int i = 1; i < numIntervals; i++) {
            vector<int> interval = intervals[i];
            int first = interval[0];
            int second = interval[1];

            if (first <= finish) {
                finish = max(finish, second);
                res[N][1] = finish;
            } else {
                N++;
                start = first;
                finish = second;
                res.push_back({ start, finish });
            }
        }
        return res;
    }
};
