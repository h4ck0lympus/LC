// lc:local-begin (removed before sending to LeetCode)
pub struct Solution;
// lc:local-end
impl Solution {
    pub fn jump(nums: Vec<i32>) -> i32 {
        let n = nums.len();
        if n == 1 {
            return 0;
        }
        let mut curr_end = 0;
        let mut farthest = 0;

        let mut jumps = 0;
        for curr in 0..n {
            let jump_size: usize = nums[curr] as usize;
            farthest = farthest.max(curr + jump_size);
            if curr == curr_end {
                jumps+=1;
                curr_end = farthest;
                if farthest >= n-1 {
                    return jumps;
                }
            }
        }
        -1
    }
}
