#include <bits/stdc++.h>

using namespace std;

class Solution {
public:
    bool isSubset(map<char,int>bmap,  string a) {
        map<char, int> amap;
        for (char c: a) {
            amap[c]++;
        }
        for (auto e: bmap) {
            if (amap[e.first] < e.second) {
                return false;
            }
        }
        return true;
    }
    vector<string> wordSubsets(vector<string>& words1, vector<string>& words2) {
        vector<string> res;
        map<char, int> max_bmap;
        for (string b: words2) {
            map<char, int> bmap;
            for (char c: b) {
                bmap[c]++;
            }

            for (auto e: bmap) {
                max_bmap[e.first] = max(max_bmap[e.first], e.second);
            }
        }
        for (string a: words1) {
            if (isSubset(max_bmap, a)) {
                res.push_back(a);
            }
        }
        return res;
    }
};
