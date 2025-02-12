class Solution {
  public:
    void ss(vector<int>& arr, vector<int>& res, int i, int sum) {
        if (i >= arr.size()) {
            res.push_back(sum);
            return;
        } else {
            // we can either take the element at i or skip it
            
            // taking it
            ss(arr, res, i+1, sum + arr[i]);

            // not taking it
            ss(arr, res, i+1, sum);
        }
    }
    vector<int> subsetSums(vector<int>& arr) {
        vector<int> res;
        int i = 0;
        int sum = 0;
        ss(arr, res, i, sum);
        return res;
    }
};
