class Solution {
  public:
    void bitManipulation(int num, int i) {
        // your code here
        num = num & (1 << i);
        cout << num << " ";
        num = num | (1 << i); 
        cout << num << " ";
        num = num & ~(1 << i);
        cout << num << " ";
    }
};

