class Solution {
public:
    long long minOperations(vector<int>& nums) {
      // locally taken decision might affect our result globally - dp ???
      // greedy ?? 
      int n = nums.size();
      long long ans = 0;
      for (int i=n-2; i >= 0; i--) {
        if (nums[i] > nums[i+1]) ans += nums[i] - nums[i+1];
      }
      // cout << ans << "\n";
      return ans;
    }
};
