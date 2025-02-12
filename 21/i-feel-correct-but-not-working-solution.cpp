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

        ListNode* l1 = list1;
        ListNode* l2 = list2;

        while (l1 != nullptr && l2 != nullptr) {
            int first = l1->val;
            int second = l2->val;

            if (first <= second) {
                ListNode* temp = l1->next;
                l1->next = l2;
                l1 = temp;
            } else {
                ListNode* temp = l2->next;
                l2->next = l1;
                l2 = temp;
            }
        }

        if (l1 == nullptr && l2 != nullptr) {
            l1->next = l2;
        } else if (l2 == nullptr && l1 != nullptr) {
            l2->next = l1;
        }

        if (list1->val <= list2->val) {
            return list1;
        }
        return list2;
    }
};
