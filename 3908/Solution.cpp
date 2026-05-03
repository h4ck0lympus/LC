class Solution {
public:
    bool validDigit(int n, int x) {
      int last_num = -1;
      bool ok = false;
      while (n) {
        int l = n % 10;
        last_num = l;
        if (l == x) ok = true;
        n /= 10;
      }
      return (ok && last_num != x);
    }
};
