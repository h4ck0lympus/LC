typedef long long ll;
class Solution {
public:
    ll eatingSpeed(vector<int>& piles, ll hourly_speed) {
        long total = 0;
        for (int num: piles) {
            total += ceil((double) num / (double) hourly_speed );
        }
        return total;
    }

    ll  minEatingSpeed(vector<int>& piles, int h) {
        // answer range = [1, max(piles)] => [1, 11]
        ll start = 1, end = *max_element(piles.begin(), piles.end());
        ll ans = end;

        while (start <= end) {
            ll mid = start + (end - start) / 2;
            ll hours_taken = eatingSpeed(piles, mid);
            if (hours_taken <= h)  {
                ans = mid;
                end = mid - 1;
            } else {
                start = mid + 1;
            }
        }
        return ans;
    }
};
