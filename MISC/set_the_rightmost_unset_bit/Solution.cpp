class Solution {
  public:
    int setBit(int n) {
        // Write Your Code here
        int i=0;
        while (n & (1 << i)) {
                i++;
        }
        n |= (1 << i);
        return n;
    }
};
