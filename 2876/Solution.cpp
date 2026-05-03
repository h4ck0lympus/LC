// first we detect if a node is part of cycle
// if a node is inside a cycle, ans for that node = len of cycle
// for a node outside of a cycle, ans = len TO cycle + len OF cycle 

class Solution {
public:
    Solution() {
      ios_base::sync_with_stdio(false);
      cin.tie(0);
      cout.tie(0);
    }
    vector<int> countVisitedNodes(vector<int>& edges) {
      int n = edges.size(); // num vertices and edges
      vector<int> res(n, 0);

      // state[n] = 0 - still needs to be processed
      // state[n] = 1 - active in the current dfs run
      // state[n] = 2 - has been processed

      vector<int>state(n);
      vector<bool> vis(n, 0);
      vector<int> parent(n, -1);

      function<void(int, int)> dfs;
      dfs = [&](int node, int p) {
        if (vis[node]) {
          if (state[node] == 1)  {
            // cycle detected
            int cyclen = 1;
            int cur = p;
            // while we have not reached the repeated node (1 -> 2 -> 3 -> 1 )
            //                                                        ^
            while (cur != node) {
              cyclen++;
              cur = parent[cur];
            }
            cur = p;
            while (cur != node) {
              res[cur] = cyclen;
              cur = parent[cur];
            }
            res[node] = cyclen;
          } 
          return;
        }
        vis[node] = 1;
        state[node] = 1;
        parent[node] = p;
        int next = edges[node];
        dfs(next, node);
        if (res[node] == 0) {
          res[node] = 1 + res[next];
        }
        state[node] = 2;
      };

      for (int i=0; i < n; i++) {
        if (!vis[i]) {
          dfs(i, -1);
        }
      }
      return res;
    }
};
