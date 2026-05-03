// in theory we can also do this using graphs but there will be a lot of edges (1e4 * 1e4 * 1e5)
// then another theory is that we can approach this using greedy ?
#pragma GCC optimize ("-O3")
#pragma GCC optimize("O3,unroll-loops")
#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")
static int fast_io = [] {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    return 0;
}();

class Solution {
public:
    __attribute__((no_sanitize("address", "thread")))
    bool canJump(vector<int>& nums) {
      int n = nums.size();
      if (n == 1) return true;
      // what's the farthest point i can reach - greedy
      int f=0; // we can always reach first idx
      for (int i=0; i < n; i++) {
        if (i > f) return false;
        f = max(f, i + nums[i]); 
        if (f >= n-1) return true;
      }
      return (f >= n-1);
    }
};
