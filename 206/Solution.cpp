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

#include <bits/stdc++.h>

using namespace std;

class Solution {
public:
    Solution()
    {
        ios_base::sync_with_stdio(false);
        cin.tie(NULL);
        cout.tie(NULL);
    }

    ListNode* reverse(ListNode* prev, ListNode* curr)
    {
        if (curr == nullptr) {
            return prev;
        } else {
            ListNode* next = curr->next;
            curr->next = prev;
            return reverse(curr, next);
        }
    }

    ListNode* reverseList(ListNode* head)
    {
        if (head == nullptr || head->next == nullptr) {
            return head;
        } else {
            return reverse(nullptr, head);
        }
    }
};
