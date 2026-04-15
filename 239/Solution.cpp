#include "bits/stdc++.h"

using namespace std;
class Solution {
public:
    vector<int> maxSlidingWindow(vector<int>& nums, int k) {
        int n = nums.size();
        vector<int> res; 
        // only storing idxs in window so we know when it goes out of the window
        deque<int> window;
        for (int i=0; i < n; i++) {
            // if curr element is bigger than last few added elements, pop elements that are lesser
            // otherwise if curr element is smaller, then just push since it's possible next max of next window
            while (!window.empty() && nums[i] > nums[window.back()]) window.pop_back();
            window.push_back(i);
            // also clear elements outside window 
            while (!window.empty() && window.front() < i-k+1) window.pop_front();
            // only push element when window size is achieved (for first couple of iterations)
            if (i >= k-1) res.push_back(nums[window.front()]);
        }
        return res;
    }
};

/*
    [3,3,5,5,6,7]
    [3,3,5]
*/
