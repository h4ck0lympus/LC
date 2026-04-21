#include "bits/stdc++.h"
using namespace std;

int __speed = [] {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    return 0;
}();

/**
 * Definition for a binary tree node.
 */
 struct TreeNode {
     int val;
     TreeNode *left;
     TreeNode *right;
     TreeNode() : val(0), left(nullptr), right(nullptr) {}
     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 };

class Solution {
public:
    bool sym(TreeNode* r1, TreeNode* r2) {
        if (r1 == nullptr && r2 == nullptr) {
            return true;
        }

        if (r1 == nullptr && r2) return false;
        if (r2 == nullptr && r1) return false;

        if (r1->val != r2-> val) {
            return false;
        } 

        return sym(r1->left, r2->right) && sym(r1->right, r2->left);
    }
    bool isSymmetric(TreeNode* root) {
      return sym(root, root);
    }
};
