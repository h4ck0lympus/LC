#include <bits/stdc++.h>

using namespace std;

class Solution {
public:
    void merge(vector<int>& nums1, int m, vector<int>& nums2, int n)
    {
        int max_idx = m + n - 1;
        int first = m - 1;
        int second = n - 1;

        while (first >= 0 && second >= 0) {
            if (nums1[first] >= nums2[second]) {
                nums1[max_idx--] = nums1[first--];
            } else {
                nums1[max_idx--] = nums2[second--];
            }
        }

        while (second >= 0) {
            nums1[max_idx--] = nums2[second--];
        }
    }
};
