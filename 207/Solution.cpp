class Solution {
public:
    // n = num of course
    // p = prerequisites
    bool canFinish(int n, vector<vector<int>>& p) {
      vector<int> adj[n];
      // make adj matrix based on that 
      vector<int> indegree(n);
       for (vector<int>& q: p) {
         // prerequisites[i] = [ai, bi]
         adj[q[1]].push_back(q[0]);  // gotta take bi before doing ai
         indegree[q[0]]++;
       }
      queue<int> q;
      for (int i=0; i < n; i++) {
        if (indegree[i] == 0) q.push(i);
      }
      vector<int> order;
      while (!q.empty()) {
        int curr = q.front(); q.pop();
        order.push_back(curr);
        for (int u: adj[curr]) {
          indegree[u]--;
          if (indegree[u] == 0) q.push(u);
        }
      }
      return order.size() == n;
    }
};
