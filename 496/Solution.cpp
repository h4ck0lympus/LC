#include "bits/stdc++.h"
using namespace std;
class Solution {
public:
    vector<int> nextGreaterElement(vector<int>& nums1, vector<int>& nums2) {
        vector<int> st;
        map<int, int> mp; // element -> idx
        int mx = nums2[0]; mp[nums2[0]] = 0;
        for (int i=1; i < nums2.size(); i++) {
            if (mx < nums2[i])  {
                st.push_back(nums2[i]);
                mp[nums2[i]] = i;
            }
        }
    }
};
