class Solution {
  public:
    int kthElement(vector<int>& a, vector<int>& b, int k) {
        int first = 0, second = 0;
        int idx = 0;

        while (first < a.size() && second < b.size()) {
            if (a[first] <= b[second]) {
                if (idx == k - 1) return a[first];
                first++;
            } else {
                if (idx == k - 1) return b[second];
                second++;
            }
            idx++;
        }

        while (first < a.size()) {
            if (idx == k - 1) return a[first];
            idx++;
            first++;
        }

        while (second < b.size()){
            if(idx == k - 1) return b[second];
            idx++;
            second++;
        }

        return -1;
    }
};

