#include <bits/stdc++.h>

using namespace std;

bool weFitCows(vector<int>& arr, int cows, int distance)
{
    cows--; // first cow will always go in first stall, so first cow is always accomodated
    int prev = 0;
    for (int i=1; i < arr.size(); i++) {
        if (arr[i] - arr[prev] >= distance) {
            cows--;
            prev = i;
        }

        if (cows == 0) return true; // if all cows are accomodated
    }
    return false;
}

int Solution(vector<int>& arr, int C)
{
    sort(arr.begin(), arr.end());
    int n = arr.size();
    int low = arr[0], high = arr[n - 1] - arr[0];

    while (low <= high) {
        int mid = low + (high - low) / 2; // minimum number of distance between 2 cows so we can fit them easily
        if (weFitCows(arr, C, mid)) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    return high;
} 

int main (void) {
    cin.tie(nullptr);
    cout.tie(nullptr);
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) {
        int N, C, loc;
        vector<int> stall_location;
        cin >>  N >> C;
        for (int i=0; i < N; i++) {
            cin >> loc;
            stall_location.push_back(loc);
        }
        cout << Solution(stall_location, C) << endl;
    }
    return 0;
}
