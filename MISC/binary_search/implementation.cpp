//{ Driver Code Starts
#include <bits/stdc++.h>
using namespace std;


// } Driver Code Ends
class Solution {
  public:
    bool search(int start, int end, vector<int>& arr, int k) {
        if (start > end){
            return false; 
        }
        
        int mid = start + (end - start) / 2;

        if (arr[mid] == k) {
            return true;
        } else if (arr[mid] < k) {
            // look right
            return search(mid + 1, end, arr, k);
        } else {
            // look left
            return search(start, mid - 1, arr, k);
        }
    }
    // Function to find element in sorted array
    // arr: input array
    // k: element to be searched
    
    bool searchInSorted(vector<int>& arr, int k) {
        // Your code here
        return search(0, arr.size() - 1, arr, k);
    }
};

//{ Driver Code Starts.

int main(void) {

    int t;
    cin >> t;
    cin.ignore();
    while (t--) {

        vector<int> arr;
        string input;
        getline(cin, input);
        stringstream ss(input);
        int number;
        while (ss >> number) {
            arr.push_back(number);
        }

        int k;
        cin >> k;
        cin.ignore();
        Solution ob;
        cout << (ob.searchInSorted(arr, k) ? "true" : "false") << endl;
        cout << "~" << endl;
    }

    return 0;
}

// } Driver Code Ends
