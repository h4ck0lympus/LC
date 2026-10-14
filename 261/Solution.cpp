#include "bits/stdc++.h"
using namespace std;
class Solution {
public:

    bool has_cycle(vector<vector<int>>& adj, vector<bool>& vis, int u, int p) {
        // contains cycle or not
        vis[u] = 1;

        for (int v: adj[u]) {
            if (!vis[v]) {
                if (has_cycle(adj, vis, v, u)) {
                    return true;
                }
            } else if (v != p) {
                return true;
            }
        }
        return false;
    }

    bool validTree(int n, vector<vector<int>>& edges) {
        // Your code goes here
        
        // there should be n nodes and n-1 edges
        if (edges.size() != n-1) return false;
        
        vector<vector<int>> adj(n);
        vector<bool> vis(n);
        for (vector<int>& e: edges) {
            int u = e[0], v = e[1];
            adj[u].push_back(v);
            adj[v].push_back(u);
        }
        // check cycle
        if (has_cycle(adj, vis, 0, -1)) return false;

        // every node should be approachable
        for (int i=0; i < n; i++) {
            if (!vis[i]) return false;
        }

        return true;
    }
};
