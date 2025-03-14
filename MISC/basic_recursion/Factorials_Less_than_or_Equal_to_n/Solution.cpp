class Solution {
  public:
    long long factorial(int x) {
        if (n <= 1)
            return 1;
        else
            return x * factorial(x - 1);
    }
    vector<long long> factorialNumbers(long long n) {
        vector<long long> res;
        int i=1;
        while (true) {
            int num = factorial(i);
            if (num <= n)
                res.push_back(num);
            else
                break;
            i++;
        }
        return res;
    }
};
