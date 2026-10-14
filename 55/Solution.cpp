class Solution {
public:
    bool canJump(vector<int>& nums) {
      int n = nums.size();
      if (n == 1) return true;
      int farthest = 0; // farthest i can reach from curr cell
      for (int curr=0; curr < n; curr++) {
        if (curr > farthest) return false; // no path exist to reach curr with previous routes
        int step_size = nums[curr];
        int curr_farthest = curr + step_size;
        farthest = max(farthest, curr_farthest);
        if (farthest >= n-1) return true;
      }
      return false;
    }
};
