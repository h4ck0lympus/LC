struct DSU {
  vector<int> e;
  int N;
  DSU(int n) {
    e.resize(n, -1);
    N = n;
  } 
  int find(int u) {
    if (e[u] < 0) return u;
    e[u] = find(e[u]);
    return e[u];
  }
  int size(int u) {
    return -e[find(u)];
  }
  void join(int u, int v) {
    int pu = find(u), pv = find(v);
    if (pu == pv) return;
    if (size(pu) > size(pv)) swap(pu, pv);
    e[pv] += e[pu];
    e[pu] = pv;
    N--;
  }
};

class Solution {
public:
    int findCircleNum(vector<vector<int>>& isConnected) {
      int n = isConnected.size();
      DSU dsu(n);
      for (int i=0; i < n; i++) {
        for (int j=0; j < n; j++) {
          if (i == j) continue;
          if (isConnected[i][j]) dsu.join(i, j);
        }
      }
      return dsu.N;
    }
};
