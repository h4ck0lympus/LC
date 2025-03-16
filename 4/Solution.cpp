// TC = O(log(min(n, m)))
// SC = O(1) 
class Solution {
public:
    double findMedianSortedArrays(vector<int>& a, vector<int>& b) {
        int n1 = a.size(), n2 = b.size();
        if (n1 > n2) return findMedianSortedArrays(b, a); // taking smaller array because that way we want min TC
        int low = 0, high = n1;
        int left = (n1 + n2 + 1) / 2; // number of elements required on left side of array
        int n = (n1 + n2); // total number of elements when both arr are combined
        while (low <= high) {
            int mid1 = (high + low) >> 1; // (high + low) / 2; (number of elements from smaller array)
            int mid2 = left - mid1;  // number of elements from bigger array
                                     
            int l1 = INT_MIN, l2 = INT_MIN;
            int r1 = INT_MAX, r2 = INT_MAX;

            if (mid1 < n1) r1 = a[mid1];
            if (mid2 < n2) r2 = b[mid2];
            if (mid1 - 1 >= 0) l1 = a[mid1 - 1];
            if (mid2 - 1 >= 0) l2 = b[mid2 - 1];
            

            if (l1 <= r2 && l2 <= r1) {
                if (n % 2) {
                    return max(l1, l2);
                } else {
                    return (double)((max(l1, l2) + min(r1, r2))) / 2.0;
                }
            } else if (l1 > r2) {
                high = mid1 - 1; // move left
            } else {
                low = mid1 + 1; // otherwise move to right
            }
        }

        return 0;
    }
}; 
