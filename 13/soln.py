#!/usr/bin/env python3 

class Solution:
    def romanToInt(self, s: str) -> int:
        val = {
            "I": 1, 
            "V": 5, 
            "X": 10, 
            "L": 50, 
            "C": 100, 
            "D": 500, 
            "M": 1000
        }
        
                
        res = val[s[len(s)-1]]  
        # from end to start
        for i in range(len(s)-2,-1,-1) : 
            if val[s[i]] >= val[s[i+1]] : 
                res += val[s[i]]
            else : 
                res -= val[s[i]] 

        return res 


if __name__ == '__main__' : 
    sol = Solution() 
    print(sol.romanToInt("MCMXCIV"))
            

