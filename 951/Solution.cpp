#include "bits/stdc++.h"
using namespace std;

int __speed = [] {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    return 0;
}();

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};


// FOCKING INVERSE BINARY TREE !!!@@@!#e$!@#!@#!
class Solution {
public:
    bool inv(TreeNode* r1, TreeNode* r2) {
        if (!r1 && !r2) return true;

        if (r1 && !r2) return false;
        if (!r1 && r2) return false;

        if (r1->val != r2->val) return false;

        return (inv(r1->right, r2->right) && inv(r1->left, r2->left)) || \
               (inv(r1->right, r2->left) && inv(r1->left, r2->right)); 
    }
    bool flipEquiv(TreeNode* root1, TreeNode* root2) {
        return inv(root1, root2);
    }
};
