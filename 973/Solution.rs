// lc:local-begin (removed before sending to LeetCode)
pub struct Solution;
// lc:local-end
fn dist(point: &Vec<i32>) -> f32 {
    ((point[0]*point[0] + point[1]*point[1]) as f32).sqrt()
}

impl Solution {
    pub fn k_closest(mut points: Vec<Vec<i32>>, k: i32) -> Vec<Vec<i32>> {
        points.sort_by(|p1, p2| dist(p1).total_cmp(&dist(p2)));
        points.truncate(k as usize);
        points
    }
}

