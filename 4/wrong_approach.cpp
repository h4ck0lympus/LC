class Solution {
public:
    int elementsOnLeft(vector<int>& nums1, vector<int>& nums2, int m) {
        int count = 0;
        for (int num: nums1) {
            count += (num < m);
        }

        for (int num: nums2) {
            count += (num < m);
        }

        return count;
    }

    double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2) {
        int n1 = nums1.size(), n2 = nums2.size();

        double low = (double)min(nums1[0], nums2[0]); // min of both arrays
        double high = (double)max(nums1[n1 - 1], nums2[n2 - 1]); // max of both arrays

        while (low <= high) {
            double median = (low + (high - low) / 2);
            int e1 = elementsOnLeft(nums1, nums2, median);
            int e2 = (n1 + n2) / 2;

            if (e1 == e2) {
                return median;
            } else if (e1 < e2) {
                low = median + 1;
            } else {
                high = median - 1;
            }
        }

        return (low + high) / 2;
    }
};
