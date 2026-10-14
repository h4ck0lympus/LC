#include "bits/stdc++.h"
using namespace std;
class Solution {
public:
    vector<vector<int>> findPrimePairs(int n) {
        if (n <= 2) return {};
        vector<char> sieve(n+1, true);
        sieve[0] = false; sieve[1] = false;
        for (int i=2; i * i < n ; i++) {
            if (sieve[i]) {
                for (int j=i*2; j < n; j+=i) {
                    sieve[j] = false;
                }
            }
        }

        vector<vector<int>> res;
        for (int i=0; i <= n/2; i++) {
            if (sieve[i]) {
                int comp = n - i;
                if (sieve[comp]) {
                    res.push_back({i, comp});
                }
            }
        }
        return res;
    }
};
