#include "bits/stdc++.h"
using namespace std;
class Solution {
public:
    static bool comp(string num1, string num2) {
        return (num1+num2 > num2+num1);
    }
    string largestNumber(vector<int>& nums) {
        vector<string> fuck;
        for (int num: nums) {
            fuck.push_back(to_string(num));
        }
        sort(fuck.begin(), fuck.end(), comp);
        if (fuck[0] == "0") return "0";
        string cum= "";
        for (string num: fuck) {
            cum += num;
        }
        return cum;
    }
};

