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
    vector<int> postorderTraversal(TreeNode* root) {
        if (root == nullptr) {
            return {};
        }

        vector<TreeNode*> stack;
        vector<int> visited;
        stack.push_back(root);

        while (!stack.empty()) {
            TreeNode* node = stack.back();
            stack.pop_back();
            if (node->right) {
                stack.push_back(node->right);
            }

            if (node->left){
                stack.push_back(node->left);
            }

            visited.push_back(node->val);
        }

        return visited;
    }
};
