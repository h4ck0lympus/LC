#include <bits/stdc++.h>

using namespace std;

class Solution {
public:
    Solution()
    {
        ios_base::sync_with_stdio(false);
        cin.tie(NULL);
        cout.tie(NULL);
    }
    int majorityElement(vector<int>& nums)
    {
        int count = 0;
        int element = 0;

        // bool elementSet = false;

        int N = nums.size();
        for (int num : nums) {
            if (count == 0) {
                element = num;
            }

            if (num == element) {
                count++;
            } else {
                count--;
            }
        }

        return element;
    }
};

int main(int argc, char* argv[])
{
    vector<int> nums = { 3, 2, 3 };
    Solution s = *new Solution();
    cout << s.majorityElement(nums) << endl;
    return 0;
}
