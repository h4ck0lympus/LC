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

    vector<int> twoSum(vector<int>& nums, int target)
    {
        map<int, int> record;
        int N = nums.size();

        for (int i = 0; i < N; i++) {
            int reqd = target - nums[i];
            if (record.find(reqd) != record.end()) {
                return { i, record[reqd] };
            }
            record[nums[i]] = i;
        }

        return {};
    }
};

int main(int argc, char* argv[])
{
    Solution s = *new Solution();
    vector<int> v = { 3, 2, 4 };
    vector<int> res = s.twoSum(v, 6);
    for (int i : res) {
        cout << i << " ";
    }
    cout << endl;
    return 0;
}
