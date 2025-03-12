int numOfStudentsAssigned(vector<int>& arr, int max_pages)
{
    int cs = 0, count = 1; // atlease one student will read one book

    for (int i=0; i < arr.size(); i++){
        if (cs + arr[i] <= max_pages) {
            cs += arr[i];
        } else {
            cs = arr[i];
            count++;
        }
    }
    return count;
}

// arr[i] - number of pages in ith book
// n - number of books
// m - number of students
// maximum number of pages assigned to a student is minimum.
int findPages(vector<int>& arr, int n, int m) {
    if (m > n) {
        // if number of students more than books
        // allocation not possible return -1
        return -1;
    }

    // each student will atleast read one book and that should be max so max(arr) is min
    // range of answer = [max(arr), sum(arr)]
    int low = *max_element(arr.begin(), arr.end()), high = accumulate(arr.begin(), arr.end(), 0);
    
    while (low <= high) {
        // this is the maximum number of pages per student
        int max_page = low + (high - low) / 2;

        if (numOfStudentsAssigned(arr, max_page) > m)  {
            low = max_page + 1;
        } else {
            // we need to minimize this so move to left
            high = max_page - 1;
        }
    }

    return low;
}
