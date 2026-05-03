class Solution {
public:
    vector<int> minCost(vector<int>& nums, vector<vector<int>>& queries) {
      // nums is sosrted
      // because nums is sorted closest(x) is minimized with adjacent elements 
      int n = nums.size();
      vector<int> p(n), s(n);
      for (int i=0; i < n-1;i++) {
        int c = abs(nums[i] - nums[i+1]);
        if (i ==0 || abs(nums[i] - nums[i+1]) < abs(nums[i] - nums[i-1])) {
          c = 1;
        }
        p[i+1] = p[i] + c;
      }
      for (int i=n-1; i > 0; i--) {
        int c = abs(nums[i] - nums[i-1]);
        if (i ==n-1 || abs(nums[i] - nums[i-1]) <= abs(nums[i] - nums[i+1])) {
          c = 1;
        }
        s[i-1] = s[i] + c;
      }

      vector<int> res;
      for (int i=0; i < queries.size(); i++) {
        vector<int> q = queries[i];
        int l=q[0], r=q[1];
        if (l <= r) res.push_back(p[r] - p[l]);
        else res.push_back(s[r] - s[l]);
      }
      return res;;
    }
};
