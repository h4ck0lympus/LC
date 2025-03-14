class Solution {
  public:
    // Function to sort the array using bubble sort algorithm.
    // we are doing adjacent swaps in bubble sort
    void bubbleSort(vector<int>& arr) {
        int n = arr.size();
        bool sorted = false;

        while (!sorted) {
            sorted = true;
            for (int i=1; i < n; i++) {
                if (arr[i] < arr[i-1]) {
                    swap(arr[i], arr[i-1]);
                    sorted = false;
                }
            }
        }
    }
};
