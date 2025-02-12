struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

// 1 -> 2 -> 3 -> 2 -> 1
// length = 5
// midIdx = 2

class Solution {
public:
    bool isPalindrome(ListNode* head)
    {
        // if no element return false
        if (head == nullptr) {
            return false;
        }

        // single element - true
        if (head && head->next == nullptr) {
            return true;
        }

        // slower way to find index
        // find the length of LL
        // find the middle index
        // move curr to middle index
        
        // better approach (2 pointer approach):
        ListNode* slow = head; // jumps once
        ListNode* fast = head; // jumps twice
        
        while (fast != nullptr && fast->next != nullptr) {
            slow = slow->next;
            fast = fast->next->next;
        }

        // reverse second part of list
        ListNode* prev = nullptr;
        ListNode* curr = slow;
        while (curr != nullptr) {
            ListNode* next = curr->next;
            curr->next = prev;
            prev = curr;
            curr = next;

        }

        // check if first group is same as second group
        ListNode* first = head;
        ListNode* second = prev;

        // only need to check second, because second will reach nullptr first
        while (second != nullptr) {
            if (first->val != second->val) {
                return false;
            }
            first = first->next;
            second = second->next;
        }

        return true;
    } 
};
