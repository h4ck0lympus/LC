/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */
class FindElements {
public:
    unordered_set<int> elements;
    FindElements(TreeNode* root) {
        root->val = 0;
        initTree(root);
    }

    void initTree(TreeNode* root) {
        if (root == nullptr)
            return;

        if (root->left) {
            root->left->val = 2 * root->val + 1;
            elements.insert(root->left->val);
            initTree(root->left);
        }

        if (root->right) {
            root->right->val = 2 * root->val + 2;
            elements.insert(root->right->val);
            initTree(root->right);
        }
    }
    
    bool find(int target) {
        return elements.find(target) != elements.end();
    }
};

/**
 * Your FindElements object will be instantiated and called as such:
 * FindElements* obj = new FindElements(root);
 * bool param_1 = obj->find(target);
 */
