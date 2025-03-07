// bruteforce
class Solution {
public:
    int findKthPositive(vector<int>& arr, int k) {
        int count = 0;
        int j = 0;
        int i;
        for (i=1; i < INT_MAX; i++) {
            if (j < arr.size() && arr[j] == i)
                j++;
            else 
                count++;

            if (count == k)
                return i;
        }

        return i;
    }
};
