class Solution {
public:
    int minOperations(vector<int>& nums, int k) {
        // min priority queue from vector
        priority_queue<long, vector<long>, greater<long>> pq(nums.begin(), nums.end());
        int operation = 0;

        while (pq.size() >= 2 && pq.top() < k) {
            int x = pq.top(); pq.pop();
            int y = pq.top(); pq.pop();
            pq.push(min(x, y) * 2L + max(x, y));
            operation++;
        }

        return operation;
    }
};
