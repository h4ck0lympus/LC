class Solution {
  public:
    int findKRotation(vector<int> &arr) {
        // Code Here
        int start = 0, end = arr.size() - 1;
        int min_element = INT_MAX;
        int idx = -1;

        while (start <= end) {
            int mid = start + (end - start) / 2;

            if (arr[start] <= arr[mid]) {
                if (arr[start] < min_element) {
                    min_element = arr[start];
                    idx = start;
                }
                start = mid + 1;
            } else {
                if (arr[mid] < min_element) {
                    min_element = arr[mid];
                    idx = mid;
                }
                end = mid - 1;
            }
        }

        return idx;
    }
};

