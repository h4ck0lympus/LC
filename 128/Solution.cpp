#include "bits/stdc++.h"
using namespace std;

// obvious naive approach
class Solution {
public:
  int longestConsecutive(vector<int> &nums) {
    int n = nums.size();
    sort(nums.begin(), nums.end());
    int ms = 0;
    int cs = 1;
    for (int i = 0; i < n - 1; i++) {
      if (nums[i] + 1 == nums[i + 1]) {
        cs += 1;
      } else {
        ms = max(ms, cs);
        cs = 1;
      }
    }
    return ms;
  }
};
