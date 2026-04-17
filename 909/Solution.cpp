#include "bits/stdc++.h"
using namespace std;
class Solution {
public:
    int snakesAndLadders(vector<vector<int>>& board) {
        int n = board.size(), m = board[0].size();
        vector<bool> vis(n * n);
        queue<pair<int, int>> q;
        q.push({0, 0});
        auto get_pos = [&](int curr) -> pair<int, int> {
            int cr = n - (curr / n) - 1; // 1 idx to 0 idx
            int cc = ((curr / n) & 1) ? (n - (curr % n) - 1): curr % n;
            return {cr, cc};
        };
        while (!q.empty()) {
            auto [curr, rolls] = q.front(); q.pop();
            if (vis[curr]) continue;
            vis[curr] = 1;
            if (curr == n*n - 1) {
                return rolls;
            }
            auto [cr, cc] = get_pos(curr);
            for (int d=1, i=curr+1; d <= 6 && i < n * n; i++, d++) {
                auto [nr, nc] = get_pos(i);
                if (board[nr][nc] != -1) {
                    q.push({board[nr][nc] - 1, rolls+1});
                } else {
                    q.push({i, rolls+1});
                }
            }
        }

        return -1;
    }
};

// 1 = (5, 0)
// 2 = (5, 1)
// 6 = (5, 5)
// 7 = (4, 5)
// 12 - (4, 0)
// 18 - (3, 5)
// 17 - (3, 4)
// 13 - (3, 0)

