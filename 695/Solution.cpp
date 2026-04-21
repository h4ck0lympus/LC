#include "bits/stdc++.h"
using namespace std;
class Solution {
public:
  int N, M;
  vector<vector<bool>> vis;
  vector<pair<int, int>> dir = {{-1, 0}, {1, 0}, {0, 1}, {0, -1}};
  int area;
  void dfs(int r, int c, vector<vector<int>>& grid) {
    if (vis[r][c]) return;
    vis[r][c] = 1;
    area++;
    for (auto [dr, dc]: dir) {
      int nr = r + dr, nc = c + dc;
      bool inside_map = (nr >= 0 && nr < N && nc >= 0 && nc < M);
      if (inside_map && grid[nr][nc] && !vis[nr][nc]) {
        dfs(r, c, grid);
      }
    }
  }

  int maxAreaOfIsland(vector<vector<int>>& grid) {
    // can be done using both dfs and bfs       
    // let's do using bfs so i can have one single function     
    N = grid.size(), M = grid[0].size();
    vis.resize(N, vector<bool>(M, 0));
    int ans = 0;
    for (int r=0; r < N; r++) {
      for (int c=0; c < M; c++) {
        if (grid[r][c] && !vis[r][c]) {
          area = 0;
          dfs(r, c, grid);
          ans = max(ans, area);
        }
      }
    }
    return ans;
  }
};
