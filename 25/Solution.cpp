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

struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, struct ListNode *next) : val(x), next(next) {}
};


class Solution {
public:
    // reverse a linked list
    ListNode* reverse(ListNode* first, int k) {
        ListNode* prev = nullptr;
        ListNode* curr = first;
        ListNode* next = nullptr;

        for (int i=0; i < k; i++) {
            next = curr->next;
            curr->next = prev;

            prev = curr;
            curr = next;
        }
        return prev;
    }

    // get length of linked list
    int getLength(ListNode* head) {
        ListNode* temp = head;
        int length = 0;
        while (temp != nullptr)  {
            temp = temp->next;
            length++;
        }
        return length;
    }

    ListNode* reverseKGroup(ListNode* head, int k) {
        if (!head || k <= 1) {
            return head;
        }


        int length = getLength(head);

        ListNode* groupStart = nullptr;
        ListNode* groupEnd = nullptr;
        ListNode* curr = head;

        // dummy(prevGroupTail) -> 1(head) -> 2 -> .... tail
        ListNode dummy;
        dummy.next = head;
        ListNode* prevGroupTail = &dummy;

        while (length >= k) {
            groupStart = curr;
            groupEnd = curr;

            // start from i = 1, because already considering group start = curr 
            // so we will have to take n - 1 steps
            // shift groupEnd to end of group of linked list
            for (int i=1; i < k; i++) {
                groupEnd = groupEnd->next;
            }

            ListNode* nextGroupStart = groupEnd->next; // save the "joining" point
            groupEnd->next = nullptr; // disconnect from main chain
            
            ListNode* reversedGroupStart = reverse(groupStart, k);
            prevGroupTail->next = reversedGroupStart;
            groupStart->next = nextGroupStart;

            // update prevGroupTail to next position
            prevGroupTail = groupStart;
            curr = nextGroupStart
            length -= k;
        }

        return dummy.next;
    }
};
