class Solution{   
public:
    pair<int, int> get(int a, int b){
        //complete the function here
        return {a ^ (a^b), b ^ (a ^ b)};
    }
};
