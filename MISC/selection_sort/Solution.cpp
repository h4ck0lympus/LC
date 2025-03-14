class Solution {
  public:
    // Function to perform selection sort on the given array.
    // selectively finding the minimum then swapping it with first element
    void selectionSort(vector<int> &arr) {
        int n = arr.size();

        for (int i=0; i < n; i++) {
            int min_idx = i;
            
            for (int j=i+1; j < n; j++) {
                if (arr[j] < arr[min_idx]) {
                    min_idx = j;
                }
            }

            swap(arr[min_idx], arr[i]);
        }
    }
};
