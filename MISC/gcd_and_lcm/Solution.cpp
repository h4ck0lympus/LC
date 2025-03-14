class Solution {
  public:
    int GCD(int a, int b) {
        if (b == 0) {
            return a;
        }
        return GCD(b, a % b);
    }
    
    vector<int> lcmAndGcd(int a, int b) {
        int gcd;
        if (a > b)
            gcd = GCD(a, b);
        else
            gcd = GCD(b, a);

        return {(a*b) / gcd, gcd};
    }
};
