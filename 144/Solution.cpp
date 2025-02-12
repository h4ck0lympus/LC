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
    vector<int> preorderTraversal(TreeNode* root) {
        if (root == nullptr) {
            return {};
        }

        vector<TreeNode*> s;
        vector<int> visited;
        s.push_back(root);

        while (!s.empty()) {
            TreeNode* node = s.back();
            s.pop_back();
            if (node != nullptr) {
                visited.push_back(node->val);
                // right is pushed first to handle it later
                if (node->right) {
                    s.push_back(node->right);
                }

                if (node->left) {
                    s.push_back(node->left);
                }
            }
        }

        return visited;
    }
};
