/**
 * @author      : HackOlympus (zeus@hackolympus)
 * @file        : Solution
 * @created     : Thursday Sep 08, 2022 00:57:26 MST
 */

#include <bits/stdc++.h>

#define FOR(n) for (int i=0; i < n ; i++)

using ll = long long;

using namespace std ; 

class Solution {
public:
    int countOdds(int low, int high) {
        int even = (high - low) / 2;  
        if ((high-low) % 2) {
            even++; 
        } 
        return even; 
    }
};
