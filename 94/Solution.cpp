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
    vector<int> inorderTraversal(TreeNode* root) {
        if (root == nullptr) {
            return {};
        }

        TreeNode* node = root;
        vector<TreeNode*> stack = {};
        vector<int> visited;

        while (!stack.empty() || node != nullptr) {
            if (node != nullptr) {
                stack.push_back(node);
                node = node->left;
            } else {
                node = stack.back();
                stack.pop_back();
                visited.push_back(node->val);
                node = node->right;
            }
        }

        return visited;
    }
};

