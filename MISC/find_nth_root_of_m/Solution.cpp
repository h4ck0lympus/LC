class Solution {
  public:
      // applies binary search to find sqrt
      int s(int start, int end, int n, int target) {
          if (start > end)
              return end;

          int mid = start + (end - start) / 2;
          int power_of_n = 1;

          for (int i=0; i <= n; i++)
              power_of_n *= mid;

          if (power_of_n == target) {
              return mid;
          } else if (power_of_n < target) {
              return s(mid + 1, end, n, target);
          } else {
              return s(0, mid - 1, n, target);
          }
      }

      int nthRoot(int n, int m) {
          int start = 0;
          int end = n;
          return s(start, end, n, m);
      }
};

