class Solution {
public:
    map<string, int> m;
    string fdbs(string cstr, int n) {
        if (cstr.size() == n) {
            if (!m[cstr])
                return cstr;
            else
                return "";
        }
        string s1 = fdbs(cstr + "0", n);

        if (!s1.empty())
            return s1;

        string s2 = fdbs(cstr + "1", n);

        if (!s2.empty())
            return s2;

        return "";
    }

    string findDifferentBinaryString(vector<string>& nums) {
        for (string num: nums){
            m[num] = 1;
        }
        string cstr = "";
        return fdbs(cstr, nums.size());
    }
};
