// we can also use hashmap to keep track of elements found 
// we can also use set DS
vector<int> sortedArray(vector<int> a, vector<int> b) {
    vector<int> union_arr;

    int first = 0, second = 0;

    while (first < a.size() && second < b.size()) {
        int added;
        if (a[first] == b[second]) {
            added = a[first];
            first++;
            second++;
        } else if (a[first] < b[second]) {
            added = a[first];
            first++;
        } else {
            added = b[second];
            second++;
       }
        union_arr.push_back(added);
        while (a[first] == added) first++;
        while (b[second] == added) second++;
    }

    if (second == b.size()) {
        while (first < a.size()) {
            int added = a[first];
            union_arr.push_back(a[first]);
            first++;
            while (a[first] == added) first++;
        }
    }

    if (first == a.size()) {
        while (second < b.size()) {
            int added = b[second];
            union_arr.push_back(b[second]);
            second++;
            while (b[second] == added) second++;
        }
    }

    return union_arr;
}
