// greedy won't work a lot of constraints + a we need to keep track of moves as well
#pragma GCC optimize ("-O3")
static int fast_io = [] {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    return 0;
}();

class Solution {
public:
    int maxPathScore(vector<vector<int>>& grid, int K) {
      int N = grid.size(), M = grid[0].size();
      // dp[i][j][k] = max score to reach (i,j) in cost = k
      vector<vector<vector<int>>> dp(N, vector<vector<int>>(M, vector<int>(K+1, INT_MIN)));
      dp[0][0][0] = 0;
      for (int r=0; r < N; r++) {
        for (int c=0; c < M; c++) {
          if (r == 0 && c == 0) continue;
          for (int k=0; k <= K; k++) {
            int score = grid[r][c];
            int cost = (int)(score > 0);
            // coming from up
            if (r > 0) {
              if (k >= cost) dp[r][c][k] = max(dp[r][c][k], dp[r-1][c][k-cost] + score);
            }

            // coming from left
            if (c > 0) {
              if (k >= cost) dp[r][c][k] = max(dp[r][c][k], dp[r][c-1][k-cost] + score);
            }
          }
        }
      }

      int ans = -1;
      for (int i=0; i <= K; i++) {
        ans = max(ans, dp[N-1][M-1][i]);
      }
      return ans;
    }
};
