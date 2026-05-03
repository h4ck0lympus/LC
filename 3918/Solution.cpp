class Solution {
public:
    int sumOfPrimesInRange(int n) {
      int r = 0;
      int t = n;
      while (t) {
        int l = t % 10;
        r = r*10 + l;
        t /= 10;
      }
      int mn = min(n, r);
      int mx = max(n, r);
      vector<bool> sieve(mx + 1, 1);
      sieve[0] = sieve[1] = false;
      for (int i=2; i <= mx; i++) {
        if (sieve[i]) {
          for (int j=i*i; j <= mx; j += i) {
            sieve[j] = 0;
          }
        }
      }
      long long res = 0;
      for (int i=mn; i <= mx; i++) {
        if (sieve[i])  res += i;
      }
      return res;
    }
};
