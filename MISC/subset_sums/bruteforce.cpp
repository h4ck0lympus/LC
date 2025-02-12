class Solution {
public:
//    int getsum(vector<int>& arr, int i) {
//        int sum = 0;
//        int idx = 0;
//        while (i) {
//            if (i & 1) {
//                sum += arr[idx];
//            }
//            i = i >> 1;
//            idx++;
//        }
//        return sum;
//    }

    vector<int> subsetSums(vector<int>& arr) {
        vector<int> res;

        // n = 1 << arr.size() i.e. 2^n 
        // suppose [1, 2, 3], n = 3 if taking arr.size() 
        // bin(3) = 011
        // but we need to iterate till 111 which is 7
        // so n = 2^n will cover all possibilities
  
        int n = arr.size();

        for (int i=0; i < (1 << n); i++) {
            int sum = 0;
            for (int j = 0; j < n; j++) {
                if (i & (1 << j )){
                    sum += arr[j];
                }
            }

            res.push_back(sum);
        }
        return res;
    }
};
