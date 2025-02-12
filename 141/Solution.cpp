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

    bool hasCycle(ListNode* head)
    {
        if (head == nullptr) {
            return false;
        }

        ListNode* slow = head;
        ListNode* fast = head;

        while (slow != NULL && fast != NULL) {
            slow = slow->next;
            if (fast->next == nullptr || fast->next->next == nullptr) {
                return false;
            }
            fast = fast->next->next;
            if (slow == fast) {
                return true;
            }
        }

        return false;
    }
};
