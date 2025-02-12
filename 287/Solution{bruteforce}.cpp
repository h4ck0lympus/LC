#include <bits/stdc++.h>

using namespace std;

class Solution {
public:
    int findDuplicate(vector<int>& nums)
    {
        map<int, int> freq;
        int i;

        for (i = 0; i < nums.size(); i++) {
            freq[nums[i]]++;
            if (freq[nums[i]] > 1) {
                break;
            }
        }

        return nums[i];
    }
};

int main(int argc, char* argv[])
{
    Solution s = *new Solution();
    vector<int> v = {3,3,3,3,3};
    cout << s.findDuplicate(v) << endl;
    return 0;
}
