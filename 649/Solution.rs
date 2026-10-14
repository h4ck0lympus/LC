// lc:local-begin (removed before sending to LeetCode)
pub struct Solution;
// lc:local-end
impl Solution {
    pub fn predict_party_victory(senate: String) -> String {
        let (mut r,mut d)=(0,0);
        let radiant_first: bool = (senate.get(0) == "R");
        for s in senate.chars() {
            match s {
                'R' => r+=1,
                'D' => d+=1,
                _ => (),
            }
        }
        if r > d {"Radiant".to_string()} else {"Dire".to_string()}
    }
}
