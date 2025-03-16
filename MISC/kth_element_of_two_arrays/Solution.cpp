// first find median, from median we can get relative location of kth element
class Solution {
  public:
    int kthElement(vector<int>& a, vector<int>& b, int k) {
        int n1 = a.size(), n2 = b.size();
        if (n1 > n2) kthElement(b, a, k);

        int left = (n1 + n2 + 1) / 2;
        int low = 0, high = n1;

        while (low <= high) {
            int mid1 = (low + high) >> 1; // number of elements picked from first array
            int mid2 = left - mid1; // number of elements picked from second array
            
            int l1 = INT_MIN, r1 = INT_MAX;
            int l2 = INT_MIN, r2 = INT_MAX;

            if (mid1 < n1) r1 = a[mid1];
            if (mid2 < n2) r2 = b[mid2];
            if (mid1 - 1 >= 0) l1 = a[mid1 - 1];
            if (mid2 - 1 >= 0) l2 = b[mid2 - 1];

            if (l1 <= r2 && l2 <= r1) {
                // found symetery, hence the median 
                // if median index == k - 1
                if (left == k - 1) {
                    return max(l1, l2);
                } else if (left < k) {
                    // k is towards the right of median
                    low = mid1 + 1;
                } else {
                    // k is towards the left of median
                    high = mid1 - 1;
                }
            } else if (l1 > r2) {
                high = mid1 - 1;
            } else {
                low = mid1 + 1;
            }
        }
    }
};

