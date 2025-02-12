#include <bits/stdc++.h>

/*
 * [5, 4, 3, 2, 1]
 * [1, 2, 3, 4, 5] ----> [4, 5, 1, 2, 3] k = 2 
 * [1, 2, 3, 4, 5] ----> [3, 4, 5, 1, 2] k = 3 
 * [1, 2, 3, 4, 5] ----> [2, 3, 4, 5, 1] k = 4
 *
 * last k elements become first k elements
 * first size(nums) - k elements become last elements
 */

using namespace std;

class Solution {
public:
    void rotate(vector<int>& nums, int k) {
        int size = nums.size();
        k = k % size; // k should stay less than size

        // reverse the array
        for (int i=0, j=size-1; i < j; i++, j--) {
            int temp = nums[i];
            nums[i] = nums[j];
            nums[j] = temp;
        }

        // reverse first k elements
        for (int i=0, j=k-1; i < j; i++, j--) {
            int temp = nums[i];
            nums[i] = nums[j];
            nums[j] = temp;
        }

        // reverse the remaining elements
        for (int i=k, j=size-1; i < j; i++, j--){
            int temp = nums[i];
            nums[i] = nums[j];
            nums[j] = temp;
        }
    }
};

int main (int argc, char *argv[]) {
    vector<int> v {1, 2, 3, 4, 5, 6, 7};
    Solution soln;
    soln.rotate(v, 3);
    return 0;
}
