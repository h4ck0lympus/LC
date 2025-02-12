#include <bits/stdc++.h>

using namespace std;

const char* __asan_default_options()
{
    return "detect_leaks=0";
}

class Solution {
public:
    vector<vector<int>> groupThePeople(vector<int>& groupSizes)
    {
        int N = groupSizes.size();
        // hashmap
        // i = group size
        // map[i] = groups
        // map[i][...] =  group
        map<int, vector<int>> m;
        vector<vector<int>> res;

        for (int i = 0; i < N; i++) {
            int groupSize = groupSizes[i];
            m[groupSize].push_back(i);
        }

        for (auto& entry : m) {
            int groupSize = entry.first;
            vector<int> people = entry.second;

            int currentGrpSize = people.size();

            for (int i = 0; i < currentGrpSize; i += groupSize) {
                vector<int> group;
                for (int j = 0; j < groupSize; j++) {
                    group.push_back(people[i + j]);
                }
                res.push_back(group);
            }
        }

        return res;
    }

private:
    int __speed = [] {ios_base::sync_with_stdio(false);cin.tie(nullptr); cout.tie(nullptr); return 0 ; }();
};

int main(int argc, char* argv[])
{
    vector<int> groupSizes = { 3, 3, 3, 3, 3, 1, 3 };
    Solution s = *new Solution();
    vector<vector<int>> ans = s.groupThePeople(groupSizes);

    for (int row = 0; row < ans.size(); row++) {
        for (int col = 0; col < ans[row].size(); col++) {
            cout << ans[row][col] << " ";
        }
        cout << endl;
    }
    return 0;
}
