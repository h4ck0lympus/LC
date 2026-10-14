#include "bits/stdc++.h"
using namespace std;

static int fast_io = [] {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    return 0;
}();

class Solution {
public:
    vector<int> vis;
    vector<int> dist;
    int ans = -1;
    void dfs(vector<int>& edges, int cur, int par) {
      if (cur == -1 || vis[cur] == -2) return; // already processed or no outgoing edge reached
      if (vis[cur] == -1) {
        // cycle detected
        ans = max(ans, dist[par] - dist[cur] + 1);
        return;
      }
      vis[cur] = -1;
      int next = edges[cur];
      if (par == -1) dist[cur] = 1;
      else dist[cur] = dist[par] + 1;
      dfs(edges, next, cur);
      vis[cur] = -2;
    }
    int longestCycle(vector<int>& edges) {
      // vis[e] = 0 - never visited
      // vis[e] = -1 - from current dfs run
      // vis[e] = -2 - previous dfs run
      // dist[e] > 0 - distance from start
      int n = edges.size();
      vis.resize(n, 0);
      dist.resize(n, 0);
      for (int i=0; i < n; i++)  {
        if (vis[i] == 0) dfs(edges, i, -1);
      }
      return ans;
    }
};
