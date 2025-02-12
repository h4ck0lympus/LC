class Solution {
public:
    int minimizeXor(int num1, int num2) {
        // Using Brian Kernighan's Algorithm to count set bits
  
        int n = num2;
        int set_bits = 0;
        while (n) {
            n = n & (n-1);
            set_bits++;
        }

        int res = 0;
        for (int i=31; i >= 0 && set_bits > 0; i--) {
            if (num1 & (1 << i)) {
                res |= (1 << i);
                set_bits--;
            }
        }

        // if any more set bits are left
        // start from LSB
        for (int i=0; i < 32 && set_bits > 0; i++) {
            // if not LSB not set, set it
            if ((res & (1 << i)) == 0) {
                res |= (1 << i);
                set_bits--;
            }
        }

        return res;
    }
};
