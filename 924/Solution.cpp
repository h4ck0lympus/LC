int __speed = [] {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    return 0;
}();

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
  bool same(int u, int v) {
    return find(u) == find(v);
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
    int minMalwareSpread(vector<vector<int>>& graph, vector<int>& initial) {
      int n = graph.size();
      DSU dsu(n);
      for (int i=0; i < n; i++) {
        for (int j=0; j < n; j++) {
          if (graph[i][j]) dsu.join(i, j);
        }
      }
      sort(initial.begin(), initial.end());
      vector<int> infected(n);
      for (int i: initial) {
        infected[dsu.find(i)]++;
      }
      int ans = initial[0];
      int best = -1;
      for (int i=0; i < initial.size(); i++) {
        int node = initial[i];
        int root = dsu.find(node);
        if (infected[root] == 1) { // only check for nodes having only one initial node, otherwise first node is selected
          int sz = dsu.size(root);
          if (sz > best) {
            best = sz;
            ans = node;
          }
        }
      }
      return ans;
    }
};


// from initial nodes, we first check which nodes correspond to unique components and which components have multiple affected initial nodes connected to them, we save this info, we only check the components with single affected initial nodes and choose the component with max size 

// 0 - 1 - 2 - 3 -4 component size = 5
// initial = [0, 4]
// removing 0 or 4 won't help since component size will remain 5 even if we remove both nodes, nothing is gonna happen
