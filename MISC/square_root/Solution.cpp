class Solution {
  public:
      // applies binary search to find sqrt
      int s(int start, int end, int target) {
          if (start > end)
              return end;

          int mid = start + (end - start) / 2;
          int square = mid * mid;

          if (square == target) {
              return mid;
          } else if (square < target) {
              return s(mid + 1, end, target);
          } else {
              return s(0, mid - 1, target);
          }
      }

      int floorSqrt(int n) {
          int start = 0;
          int end = n;
          return s(start, end, n);
      }
};
