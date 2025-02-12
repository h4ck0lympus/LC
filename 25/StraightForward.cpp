/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode() : val(0), next(nullptr) {}
 *     ListNode(int x) : val(x), next(nullptr) {}
 *     ListNode(int x, ListNode *next) : val(x), next(next) {}
 * };
 */
class Solution {
public:
    ListNode* reverseKGroup(ListNode* head, int k) {
        if (head == nullptr || k == 1)
            return head;
        int count = 0;
        ListNode* p = head;
        while (p) {
            p = p->next;
            count++;
        }
        int grps = count/k;

        // use dummy node to keep count
        ListNode* dummy = new ListNode();
        dummy->next = head;
        ListNode* prev = dummy;
        ListNode* cur = head;
        ListNode* next = cur->next;

        // to keep the count of processed groups
        count = 0;
        while (true) {
            for (int i = 1; i < k; i++) {
                // reverse linked list
                cur->next = next->next;
                next->next = prev->next;
                prev->next = next;
                next = cur->next;
            }
            count++;
            if (count == grps) break;
            prev = cur;
            cur = next;
            next = cur->next;
        }
        return dummy->next;
    }
};
