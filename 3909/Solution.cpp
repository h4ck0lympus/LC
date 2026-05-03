// naive approach = TC = O(n) SC = O(n)
// better approach = binary search ???  --- no it can only be used to find peak element
// better approach TC = O(n), SC = O(1): 
class Solution {
public:
    int compareBitonicSums(vector<int>& nums) {
      using ll = long long;     
      int n = nums.size();
      ll s1 = nums[0], s2 = 0;
      int i=1;
      for (; i < n; i++) {
        if (nums[i] < nums[i-1]) {
          break;
        }
        s1 += nums[i];
      }
      i--; // peak is part of both arrays
      //cout << s1 << "\n";
      for (; i < n; i++) {
        s2 += nums[i];
      }
      // cout << s2 << "\n";
      if (s1 > s2) return 0;
      else if (s1 < s2) return 1;
      return -1;
    }
};
