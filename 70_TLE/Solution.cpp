// this solution throws TLE, we will need to code this using DP
// TC = O(2^n) 
// SC = O(n) ( auxilliary space from recursion call stack otherwise O(1) )
class Solution {
public:
    void cs(int position, int end, int& ways) {
        if (position == end) {
            ways++;
            return;
        }

        if (position > end)
            return;

        cs(position + 1, end, ways);
        cs(position + 2, end, ways);
    }
    int climbStairs(int n) {
        int position = 0;
        int end = n;
        int ways = 0;
        cs(position, end, ways);
        return ways;
    }
};
