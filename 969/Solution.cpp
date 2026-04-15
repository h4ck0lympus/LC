#include "bits/stdc++.h"
using namespace std;
class Solution {
public:
    vector<int> pancakeSort(vector<int>& arr) {
      vector<int> ans;
      auto flip = [&](int k) {
        if (k <= 1) return;
        reverse(arr.begin(), arr.begin() + k);
        ans.push_back(k);
      };
      int n = arr.size();
      for (int i=n; i >= 1; i--) {
        int pos = -1;
        for (int j=0; j < n; j++) {
          if (arr[j] == i) {
            pos = j;
            break;
          }
        }
        assert(pos != -1);
        if (pos == i-1) continue; // already at correct pos
        flip(pos+1);
        flip(i);
      }

      return ans;
    }
};

