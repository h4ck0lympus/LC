// lc:local-begin (removed before sending to LeetCode)
pub struct Solution;
// lc:local-end
impl Solution {
    pub fn number_of_arithmetic_slices(nums: Vec<i32>) -> i32 {
        let n = nums.len();
        if n < 3 {
            return 0; // arithmetic slice not possible
        }
        let mut curr = 0;
        let mut ans = 0;
        for i in 2..n {
            let cdiff = nums[i] - nums[i-1];
            let ldiff = nums[i-1] - nums[i-2];
            if cdiff == ldiff {
                curr += 1;
                ans += curr;
            } else {
                curr = 0;
            }
        }
        ans as i32
    }
}

// 1, 2, 3, 4, 5
// [1, 2, 3]
// [2, 3, 4]
// [3, 4, 5]
// [1,2,3,4]
// [2,3,4,5]
// [1,2,3,4,5]
