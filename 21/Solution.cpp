#include <bits/stdc++.h>

struct ListNode {
    int val;
    ListNode* next;
    ListNode()
        : val(0)
        , next(nullptr)
    {
    }
    ListNode(int x)
        : val(x)
        , next(nullptr)
    {
    }
    ListNode(int x, ListNode* next)
        : val(x)
        , next(next)
    {
    }
};

using namespace std;

class Solution {
public:
    Solution()
    {
        ios_base::sync_with_stdio(false);
        cin.tie(NULL);
        cout.tie(NULL);
    }

    ListNode* mergeTwoLists(ListNode* list1, ListNode* list2)
    {
        if (list1 == nullptr && list2 == nullptr) {
            return nullptr;
        } else if (list1 == nullptr) {
            return list2;
        } else if (list2 == nullptr) {
            return list1;
        }

        ListNode* newHead = new ListNode();
        ListNode* temp = newHead;
        ListNode* l1 = list1;
        ListNode* l2 = list2;

        while (l1 != nullptr && l2 != nullptr) {
            int first = l1->val;
            int second = l2->val;

            if (first <= second) {
                temp->next = l1;
                temp = temp->next;
                l1 = l1->next;
            } else {
                temp->next = l2;
                temp = temp->next;
                l2 = l2->next;
            }
        }

        if (l1 != nullptr) {
            temp->next = l1;
        } else if (l2 != nullptr) {
            temp->next = l2;
        }

        return newHead->next; // skip dummy
    }
};
