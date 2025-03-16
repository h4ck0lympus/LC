/*
struct Node {
    int data;
    struct Node *next;
    Node(int x) {
        data = x;
        next = NULL;
    }
};
*/

class Solution {
  public:
    // Function to find the length of a loop in the linked list.
    int countNodesinLoop(Node *head) {
        Node* slow = head;
        Node* fast = head;
        bool loop_found = false;

        // check if loop is present
        while (fast != nullptr && fast->next != nullptr) {
            slow = slow->next;
            fast = fast->next->next;
            if (slow == fast) {
                // if loop present
                int count = 1;
                Node* count_ptr = slow->next;
                while (count_ptr != slow) {
                    count++;
                    count_ptr = count_ptr->next;
                }
                return count;
            }
        }

        return 0; // when loop is not present, loop length is 0
    }
};

