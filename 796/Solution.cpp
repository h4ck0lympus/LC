class Solution {
public:
    bool rotateString(string s, string goal) {
      // 2 sum vibes shit    
      int n = s.size();
      for (int i=0; i <= n; i++) {
        string pre = s.substr(0, i);
        string suf = s.substr(i);
        if (suf + pre == goal) {
          return true;
        }
      }
      return false;
    }
};
