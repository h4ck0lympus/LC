class Solution {
public:
    bool isPowerOfTwo(int n) {
        int setbit = 0;
        while (n) {
            setbit += (n & 1);
            n &= (n - 1); 
        }
        return setbit == 1;
    }
};
