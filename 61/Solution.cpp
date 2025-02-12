struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

class Solution {
public:
    ListNode* rotateRight(ListNode* head, int k) {
        if (head == nullptr) {
            return nullptr;
        }

        int size = 1;
        ListNode* curr = head;
        while (curr->next != nullptr) {
            curr = curr->next;
            size++;
        }
        curr->next = head;

        int rot = size - (k % size);

        while (rot--) {
            curr = curr->next;
        }
        head = curr->next;
        curr->next = nullptr;
        return head;
    }
};

