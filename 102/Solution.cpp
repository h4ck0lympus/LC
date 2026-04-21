#include "bits/stdc++.h"

using namespace std;

class Solution {
public:
    vector<vector<int>> levelOrder(TreeNode* root) {
        vector<vector<int>> res;
        queue<TreeNode*> q;

        if (root) q.push(root);

        while (!q.empty()) {
            vector<int> val;
            int n = q.size();
            for (int i=0; i < n; i++) {
                TreeNode* curr = q.front(); q.pop();
                val.push_back(curr->val);
                if (curr->left) q.push(curr->left);
                if (curr->right) q.push(curr->right);
            }
            res.push_back(val);
        }
        return res;
    }
};
