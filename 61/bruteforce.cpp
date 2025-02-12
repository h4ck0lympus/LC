struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

class Solution {
public:
    int getLength(ListNode* head){
        ListNode* t = head;
        int length = 0;
        while (t != nullptr) {
            t = t->next;
            length++;
        }
        return length;
    }

    ListNode* rotateRight(ListNode* head, int k) {
        if (!head) {
            return nullptr;
        }

        if (head && !head->next) {
            return head;
        }

        int L = getLength(head); // 5
        int rot = k % L; // to keep in bounds
        int count = L - rot - 1; // 3
        
        if (rot == 0) {
            return head;
        }

        ListNode* curr = head;
        while (count-- && curr->next) {
            curr = curr->next;
        }
        ListNode* next = curr->next;
        curr->next = nullptr;

        curr = next;
        while (curr && curr->next != nullptr) {
            curr = curr->next;
        }

        curr->next = head;

        return next;
    }
};

