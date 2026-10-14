// lc:local-begin (removed before sending to LeetCode)
pub struct Solution;
// lc:local-end
impl Solution {
    pub fn can_jump(nums: Vec<i32>) -> bool {
        let n = nums.len();
        let mut farthest = 0;
        for num in 0..n {
            if num > farthest {
                return false;
            }
            let curr_farthest = num + nums[num] as usize;
            farthest = curr_farthest.max(farthest);
            if farthest >= n - 1 {
                return true;
            }
        }
        return false;
    }
}
