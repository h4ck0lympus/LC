class Solution {
public:
    bool containsCycle(vector<vector<char>>& grid) {
      int n = grid.size(), m = grid[0].size();
      vector<pair<int, int>> dir = {{-1, 0}, {1, 0}, {0, 1}, {0, -1}};

      vector<vector<bool>> vis(n, vector<bool>(m, 0));
      // vector<vector<bool>> instack(n, vector<bool>(m, 0));

      bool cycle_detected = false;
      function<void(int, int, int, int)> dfs;
      dfs = [&](int r, int c, int pr, int pc) {
        vis[r][c] = 1;

        for (auto [dr, dc]: dir) {
          int nr = r + dr, nc = c + dc;
          if (!(0 <= nr && nr < n && 0 <= nc && nc < m)) continue; 
          if (grid[nr][nc] != grid[r][c]) continue;

          // skip the edge we are directly coming from (0, 0) -> (0, 1) -> (0, 0) - skip (because it looks at all 4 dirs)
          if (nr == pr && nc == pc) continue;

          // now if we reach any edge that's already visited - cycle detected
          if (vis[nr][nc]) {
            cycle_detected = 1;
            return;
          }

          dfs(nr, nc, r, c);
        }
      };

      for (int i=0; i < n; i++) {
        for (int j=0; j < m; j++) {
          if (!vis[i][j]) {
            dfs(i, j, -1, -1);
            if (cycle_detected) return true;
          }
        }
      }
      return false;
    }
};
