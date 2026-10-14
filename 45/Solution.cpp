#include "bits/stdc++.h"
using namespace std;

class Solution {
public:
    int jump(vector<int>& nums) {
        int n = nums.size();
        if (n==1) return 0; // edge-case: already at first so no jumps needed if first is last
        int jumps = 0;
        int curr=0;
        int curr_end =0;
        int farthest = 0; // farthest idx reachable using jumps+1 jumps
        while (curr < n){
            int jump_size = nums[curr];
            farthest = max(farthest, curr + jump_size);
            // we can reach curr_end in a single jump
            // farthest holds the idx we can reach farthest if we make a single jump of max jump size or less
            if (curr == curr_end) { // so if we are currently at curr_end that means we have tested all numbers till now and we will have the correct farthest
                curr_end = farthest;
                jumps++;
                if (curr_end >= n-1) return jumps;
            }
            curr++;
        }
        return -1; // should not be reachable
    }
};
