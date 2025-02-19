class Solution {
  public:
      int bs(vector<int>& arr, int start, int end, int target) {
          int mid = start + (end  - start) / 2;

          if (start > end) {
              return -1;
          }

          if (arr[mid] == target) {
              return mid;
          } else if (arr[mid] < target) {
              return bs(arr, mid+1, end, target);
          } else {
              return bs(arr, start, mid - 1, target);
          }
      }
      
      int countFreq(vector<int>& arr, int target) {
          int start = 0;
          int end = arr.size() - 1;
          int left = -1, right = -1;

          while (true) {
              int idx = bs(arr, start, end, target);
              if (idx == -1)
                  break;
              left = idx;
              end = idx - 1;
          }

          if (left == -1)
              return 0;

          start = 0;
          end = arr.size() - 1;
          while (true) {
              int idx = bs(arr, start, end, target);
              if (idx == -1)
                  break;
              right = idx;
              start = idx + 1;
          }

          return right - left + 1;
      }
};
