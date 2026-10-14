class Solution {
public:
    bool isBipartite(vector<vector<int>>& adj) {
      int n = adj.size();
      vector<int> col(n, -1);
      function<void(int, int)> dfs;
      bool ans = true;

      dfs = [&](int u, int c) {
        if (col[u] != -1) {
          if (col[u] != c) {
            ans = false; // not bipartite
          }
          return;
        }
        col[u] = c;
        for (int e: adj[u]) {
          dfs(e, c ^ 1);
        }
      };

      for (int i=0; i < n; i++) {
        if (col[i] == -1) dfs(i, 0);
      }
      return ans;
    }
};
