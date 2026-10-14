// lc:local-begin (removed before sending to LeetCode)
pub struct Solution;
// lc:local-end
// impl Solution {
//     pub fn max_profit(prices: Vec<i32>) -> i32 {
//         let n = prices.len();
//         let mut curr_min = prices[0];
//         let mut mx = 0;
//         for i in 1..n {
//             if prices[i] < curr_min {
//                 curr_min = prices[i];
//             }
//             let profit = prices[i] - curr_min;
//             // println!("profit: {profit}");
//             mx = mx.max(profit);
//         }
//         mx
//     }
// }

impl Solution {
    pub fn max_profit(prices: Vec<i32>) -> i32 {
        let n = prices.len();
        let (mut l, mut r) = (0, 1);
        let mut curr_min = prices[l];
        let mut mx = 0;
        while r < n && l < r {
            if prices[r] < curr_min {
                curr_min = prices[r];
                l = r;
            }
            let profit = prices[r] - prices[l];
            // println!("profit: {profit}");
            mx = mx.max(profit);
            r+=1;
        }
        mx
    }
}
