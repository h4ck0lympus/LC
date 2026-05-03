class Solution {
public:
    void solve(vector<vector<char>>& board) {
      int n = board.size(), m = board[0].size();
      vector<vector<bool>> vis(n, vector<bool>(m));
      function<void(int, int)> dfs;
      vector<pair<int,int>> dir = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

      dfs = [&](int r, int c) {
        if (vis[r][c] || board[r][c] != 'O') return;
        vis[r][c] = 1;
        for (auto [dr, dc]: dir) {
          int nr=r+dr, nc=c+dc;
          bool inside_board = (nr >= 0 && nr < n && nc >= 0 && nc < m);
          if (inside_board) dfs(nr, nc);
        }
      };

      for (int r = 0; r < n; r++) {
          dfs(r,0);
          dfs(r,m-1);
      }
      for (int c = 0; c < m; c++) {
          dfs(0,c);
          dfs(n-1,c);
      }

      for (int i=0; i < n; i++) {
        for (int j=0; j < m; j++) {
          if (board[i][j] == 'O' && !vis[i][j]) {
            board[i][j] = 'X';
          }
        }
      }
    }
};
