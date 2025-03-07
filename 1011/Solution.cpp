class Solution {
public:
    int shipWithinDays(vector<int>& weights, int days) {

        // minimum possible capacity must be at least the weight of the heaviest package
        // Maximum possible ship capacity should be sum of all weights 
        // shipping everything in one day
        int low = INT_MIN, high = 0; 

        for (int i=0; i < weights.size(); i++) {
            low = max(low, weights[i]);
            high += weights[i];
        }

        int ans; // minimum capacity of ship to fit all the weights and transport it in `days` days
        
        while (low <= high) {
           int capacity = low + (high - low) / 2;

            int days_tooks = 1; // start with one day (always need one day!!!)
            int sum = 0;
            for (int i=0; i < weights.size(); i++) {
                if (sum + weights[i] <= capacity) {
                    sum += weights[i];
                } else {
                    days_took++;
                    sum = weights[i];
                }
            }

           if (days_took <= days) {
                ans = capacity;
                high = capacity - 1;
            } else {
                low = capacity + 1;
            }
        }

        return ans;
    }
};
