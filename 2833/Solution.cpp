class Solution {
public:
    // one way is to bruteforce the movement 
    // another is to basically do greedy
    int furthestDistanceFromOrigin(string moves) {
      int l = 0, r = 0, d = 0; 
      for (char c: moves) {
        if (c == 'L') l++;
        else if (c == 'R') r++;
        else d++;
      }
      return max(abs(l + d - r), abs(r + d - l));
    }
};
