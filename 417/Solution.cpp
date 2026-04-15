#include "bits/stdc++.h"
using namespace std;
class Solution {
public:
    vector<pair<int,int>> dir = {{-1, 0},{1, 0}, {0, 1}, {0, -1}};
    vector<vector<int>> res;
    void dfs(int r, int c, vector<vector<int>>& grid, vector<vector<bool>>& vis) {
      int rows = grid.size(), cols=grid[0].size();
      if (vis[r][c]) return;
      vis[r][c] = 1;
      for (auto [dr, dc]: dir) {
        int nr = r + dr, nc = c + dc;
        bool inside_map = ((0 <= nr) && (nr < rows)) && ((0 <= nc) && (nc < cols));
        if (!inside_map) continue;
        bool valid = (grid[nr][nc] >= grid[r][c]);
        if (valid) {
          dfs(nr, nc, grid, vis);
        }
      }
    }

    vector<vector<int>> pacificAtlantic(vector<vector<int>>& adj) {
      int rows=adj.size(), cols=adj[0].size();
      vector<vector<bool>> vis1(rows, vector<bool>(cols, 0));
      vector<vector<bool>> vis2(rows, vector<bool>(cols, 0));

      // pacific -> atlantic
      for (int i=0; i < rows; i++) dfs(i, 0, adj, vis1);
      for (int i=0; i < cols; i++) dfs(0, i, adj, vis1);

      // atlantic -> pacific
      for (int i=0; i < rows; i++) dfs(i, cols-1, adj, vis2);
      for (int i=0; i < cols; i++) dfs(rows-1, i, adj, vis2);
      vector<vector<int>> res;
      for (int i=0; i < rows; i++) {
        for (int j=0; j < cols; j++) {
          if (vis1[i][j] && vis2[i][j]) res.push_back({i, j});
        }
      }
      return res;
    }
};
