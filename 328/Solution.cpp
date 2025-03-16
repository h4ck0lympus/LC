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

//1->2->3->4->5 odd_position = 1 current = 3
//1->3->2->4->5 odd_position = 3 current = 5
//1->3->5->2->4 

class Solution {
public:
    ListNode* oddEvenList(ListNode* head) {
        if (head == nullptr || head->next == nullptr || head->next->next == nullptr) {
            return head;
        }

        ListNode* odd_position = head; // odd node
        ListNode* even_position = head->next // even nodes 
        ListNode* current = head;

        int i = 1;

        while (current != nullptr && current->next != nullptr) { 
            ListNode* next_current = current->next;

            if (i & 1) { 
                // if odd node
                
            } else {

            }

            current = next_current;
        }

        return head;
    }
};
