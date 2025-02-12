#include <bits/stdc++.h>

using namespace std;

vector<int> findMissingRepeatingNumbers(vector<int> a)
{
    int n = a.size();
    int P = 0, Q = 0;
    int record[n];

    int expected = 0;
    int actual = 0;

    for (int i = 0; i < n; i++) {
        int num = a[i];
        record[num - 1]++;

        if (record[num - 1] == 2) {
            P = num;
        }

        expected += (i + 1);
        actual += num;
    }

    actual -= P;

    Q = expected - actual;
    vector<int> res = { P + 1, Q + 1 };
    return res;
}

int main(int argc, char* argv[])
{
    int n;
    cin >> n;
    vector<int> v;
    while (n--) {
        int t;
        cin >> t;
        v.push_back(t);
    }

    vector<int> res = findMissingRepeatingNumbers(v);
    cout << "P: " << res[0] << " Q: " << res[1] << endl;
    return 0;
}
