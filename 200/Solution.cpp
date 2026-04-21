static const bool __boost = []() {
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
	return std::ios_base::sync_with_stdio(false);
	}();

#include "bits/stdc++.h"
using namespace std;

class Solution {
public:
    vector<vector<bool>> vis;
    vector<pair<int, int>> dir = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    void dfs(int r, int c, vector<vector<char>>& grid) {
        if (vis[r][c]) return;
        vis[r][c] = 1;
        for (auto [dr, dc]: dir) {
            int nr = r + dr, nc = c + dc;
            bool in_map = ((nr >= 0) && (nr < grid.size())) && ((nc >= 0) && (nc < grid[0].size()));
            if (in_map && !vis[nr][nc] && grid[nr][nc] == '1') {
                dfs(nr, nc, grid);
            }
        }
    }
    int numIslands(vector<vector<char>>& grid) {
        int n = grid.size(), m = grid[0].size();
        vis.resize(n, vector<bool>(m, 0));
        int ans = 0;
        for (int i=0; i < n; i++) {
            for (int j=0; j < m; j++) {
                if (!vis[i][j] && grid[i][j] == '1') {
                    dfs(i, j, grid);
                    ans++;
                }
            }
        }
        return ans;
    }
};
