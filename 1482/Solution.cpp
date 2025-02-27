// TC: O(n) + O(n log n)
class Solution {
public:
    // O(n)
    int max_bloom_day(vector<int>& bloomDay)
    {
        int m = bloomDay[0];
        for (int i=1; i < bloomDay.size(); i++)
            m = max(bloomDay[i], m);
        return m;
    }

    // O(n)
    int get_possible_bouquets(vector<int>& bloomDays, int day, int k)
    {
        // k = flowers per bouquet
        int bouquets = 0;
        int flowers = 0;
        for (int bloomDay: bloomDays) {
            if (day >= bloomDay) {
                flowers++;
                if (flowers == k) {
                    bouquets++;
                    flowers = 0;
                }
            } else {
                flowers = 0;
            }
        }
        return bouquets; // bouquets possible
    }

    // O(log n)
    int minDays(vector<int>& bloomDay, int m, int k) {
        if ((long)m * k > bloomDay.size())
            return -1; // where there are not enough flowers

        // answer lies between 1 to max_element(bloomDay)
        int low = 1, high = max_bloom_day(bloomDay);
        int ans = -1;

        while (low <= high) {
            int mid = low + (high - low) / 2;
            int bouquetsPossible = get_possible_bouquets(bloomDay, mid, k);
            if (bouquetsPossible >= m) {
                ans = mid;
                high = mid - 1;
            } else {
                low = mid + 1;
            }
        }

        return ans;
    }
};
