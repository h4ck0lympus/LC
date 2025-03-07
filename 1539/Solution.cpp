class Solution {
public:
    int findKthPositive(vector<int>& arr, int k) {
        int low = 0, high = arr.size() - 1;
        
        int diff;
        while (low <= high) {
            int mid = low + (high - low) / 2;
            diff = arr[mid] - (mid + 1); // what is the difference between expected and current element ?
            
            // eliminate LHS
            if (diff < k) {
                low = mid + 1;
            } else {
                high = mid - 1;
            }
        }
 
        return low + k;
    }
};
