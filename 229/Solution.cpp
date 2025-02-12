#include <algorithm>
#include <bits/stdc++.h>
#include <climits>

using namespace std;

class Solution {
public:
    Solution()
    {
        ios_base::sync_with_stdio(false);
        cin.tie(NULL);
        cout.tie(NULL);
    }
    vector<int> majorityElement(vector<int>& nums)
    {
        int N = nums.size();
        vector<int> res;

        int e1 = INT_MIN, occ1 = 0, e2 = INT_MIN, occ2 = 0;

        for (int num : nums) {
            if (occ1 == 0 && num != e2) {
                e1 = num;
                occ1 = 1;
            } else if (occ2 == 0 && num != e1) {
                e2 = num;
                occ2 = 1;
            } else if (e1 == num) {
                occ1++;
            } else if (e2 == num) {
                occ2++;
            } else {
                occ1--;
                occ2--;
            }
        }

        if (occ1 && count(nums.begin(), nums.end(), e1) > (N / 3)) {
            res.push_back(e1);
        }

        if (occ2 && count(nums.begin(), nums.end(), e2) > (N / 3) && e1 != e2) {
            res.push_back(e2);
        }

        return res;
    }
};

int main(int argc, char* argv[])
{
    vector<int> v = { 0, 3, 4, 0 };
    Solution s = *new Solution();
    vector<int> res = s.majorityElement(v);
    for (int i : res) {
        cout << i << " ";
    }
    cout << endl;
    return 0;
}
