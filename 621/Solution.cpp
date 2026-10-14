class Solution {
public:
  int leastInterval(vector<char> &tasks, int n) {
    int freq[26] = {};
    for (char c: tasks) {
      freq[c-'A']++;
    }
    priority_queue<int> pq;
    for (int i=0; i < 26; i++) {
      if (freq[i] > 0) {
        pq.push(freq[i]);
      }
    }
    int time = 0;
    // {ready_time, freq}
    queue<pair<int, int>> ready_time;
    while (!pq.empty() || !ready_time.empty()) {
      // if ready time reached or has already passed
      if (!ready_time.empty() && ready_time.front().first <= time) {
        auto [_, f] = ready_time.front(); ready_time.pop();
        pq.push(f);
      }

      if (!pq.empty()) {
        int f = pq.top(); pq.pop();
        f--;
        if (f > 0) ready_time.push({time + n + 1, f});
      }
      time++;
    }
    return time;
  }
};

//// MATH Solution
/*
class Solution {
public:
  int leastInterval(vector<char> &tasks, int n) {
    int num_tasks = tasks.size();
    int freq[26] = {};
    int mx = INT_MIN;
    for (char c: tasks) {
      freq[c-'A']++;
      mx = max(mx, freq[c-'A']);
    }
    int cnt = count(freq, freq+26, mx);
    // max - 1 tasks 
    // mx-1 first run will claim one
    // n + 1 because after one run it's time is passed so total n+1 time + remaining cells
    int ans = max(num_tasks, (mx-1) * (n + 1) + cnt);
    return ans;
  }
};
*/

/*
["A","A","A","B","B","B"]

freq = {
    "A": 3,
    "B": 3
}

"A" -> "B" -> idle

*/
