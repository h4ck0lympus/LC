class Solution {
public:
    vector<int> findValidElements(vector<int>& nums) {
      int n = nums.size();
      vector<int> pm(n, -1), sm(n, -1);
      int mx = INT_MIN;
      for (int i=0; i < n; i++) {
        if (mx < nums[i]) {
          mx = nums[i];
          pm[i] = mx;
        }
      }
      mx = INT_MIN;
      for (int i=n-1; i >= 0; i--) {
        if (mx < nums[i]) {
          mx = max(mx, nums[i]);
          sm[i] = mx;
        }
      }
      vector<int> res;
      for (int i=0; i < n; i++) {
        if (nums[i] == pm[i] || nums[i] == sm[i]) res.push_back(nums[i]);
      }
      return res;
    }
};
