// lc:local-begin (removed before sending to LeetCode)
pub struct Solution;
// lc:local-end
impl Solution {
    pub fn earliest_full_bloom(plant_time: Vec<i32>, grow_time: Vec<i32>) -> i32 {
        // (plant, grow)
        let mut plants: Vec<(i32, i32)> = plant_time.iter().copied().zip(grow_time).collect();
        
        plants.sort_by(|first, second| second.1.cmp(&first.1));

        let mut total_time = 0;
        let mut curr_plant_time = 0;

        // for (pt, gt) in plants {
        //     if pt + gt > prev_gt {
        //         total_time += pt + gt - prev_gt; // - the overlapping grow time
        //         prev_gt = gt;
        //     } else {
        //         prev_gt = prev_gt - pt;
        //     }
        // }
        
        for (pt, gt) in plants {
            curr_plant_time += pt; // planting time has to pass, so take their collateral sum
            total_time = total_time.max(curr_plant_time + gt); // now growing time is parellel so
                                                               // all plants are growing already
                                                               // in background so total time is
                                                               // curr_plant_time + growing_time
        }

        total_time
    }
}


// (plant, grow)
//
// (1,2) (4,3) (3,1)
//
// planting time:
// 1. needs to elapse
// 2. may not be consecutive
// growing time can be overlapping
// since growing time can be overlapping we want maximum overlapping growing time so max plants can
// grow simultaenously
// BUT planting time is a constraint
// seed a: (1, 10)
// seed b: (5, 1)
// since grow time doesn't matter if it's overlapping, sort things based on growing time and start
// the process of high growing time plants first so basically the process of highest growing time
// period plants first so their planting time can be handled first and then the we can get max overlapping grow time
