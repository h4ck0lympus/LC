class Solution {
public:
    vector<int> countOppositeParity(vector<int>& nums) {
      int n = nums.size();
      int even = 0, odd = 0;
      vector<pair<int, int>> ps(n); // number of odd or even in reverse
      for (int i=n-1; i >= 0; i--) {
        ps[i] = {odd, even};
        if (nums[i] & 1) {
          odd++;
        } else {
          even++;
        }
      }
      vector<int> res;
      for (int i=0; i < n; i++) {
        if (nums[i] & 1) {
          // odd
          res.push_back(ps[i].second);
        } else {
          res.push_back(ps[i].first);
        }
      } 
      return res;
    }
};
