#include "bits/stdc++.h"
using namespace std;

struct DSU {
    vector<int> e;
    DSU(int n) {
        e.resize(n, -1);
    }
    int find(int u) {
        if (e[u] < 0) return u;
        e[u] = find(e[u]);
        return e[u];
    }
    void join(int u, int v) {
        int ru = find(u), rv = find(v);
        if (ru == rv) return;
        if (e[ru] > e[rv]) swap(ru, rv);
        e[rv] += e[ru];
        e[ru] = rv;
    } 
};

class Solution {
public:
    int numIslands(vector<vector<char>>& grid) {
        int n = grid.size(), m = grid[0].size();
        DSU d(n);
        for (int i=0; i < n; i++) {
            for (int x: grid[i]) {
                d.join(i, x);
            }
        }

        int ans = 0;
        for (int i=0; i < n; i++) {
            if (d.e[i] < 0) ans++;
        }

        return ans;
    }
};
