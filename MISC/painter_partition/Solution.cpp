int numOfBoardstime(vector<int>& boards, int time)
{
    int count = 1;
    int sc = 0;
    for (int i=0; i < boards.size(); i++){
        if (boards[i] + sc <= time) {
            sc += boards[i];
        } else {
            count++;
            sc = boards[i];
        }
    }

    return count;
}

int findLargestMinDistance(vector<int> &boards, int k)
{
    int low = *max_element(boards.begin(), boards.end());
    int high = accumulate(boards.begin(), boards.end(), 0);

    while (low <= high) {
        int time = low + (high - low) / 2;

        if (numOfBoardstime(boards, time) <= k) {
            high = time - 1;
        } else {
            low = time + 1;
        }
    }

    return low;
}
