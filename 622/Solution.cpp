class Node {
public:
    int val;
    Node* next;
    Node(): val(-1), next(nullptr) {}
    Node(int val): val(val), next(nullptr) {}
};

class MyCircularQueue {
    int size;
    int capacity;
    Node* head;
    Node* tail;
public:
    MyCircularQueue(int k) : size(0), capacity(k){
        head = new Node(); // oldest node
        tail = head; // most recent node
        head->next = tail;
    }
    
    bool enQueue(int value) {
        if (size == capacity) {
            return false;
        }
        Node* newNode = new Node(value);
        tail->next = newNode;
        tail = newNode;
        size++;
        return true;
    }

    // head->tail
    
    bool deQueue() {
        if (size == 0) {
            return false;
        }
        Node* trash = head->next;
        head->next = head->next->next;
        delete trash;
        size--;
        if (size == 0) tail = head;
        return true;
    }
    
    int Front() {
        if (size == 0) return -1;
        return head->next->val; // -1
    }
    
    int Rear() {
        if (size == 0) return -1;
        return tail->val;
    }
    
    bool isEmpty() {
        return (size == 0);
    }
    
    bool isFull() {
        return (size == capacity);
    }
};

/**
 * Your MyCircularQueue object will be instantiated and called as such:
 * MyCircularQueue* obj = new MyCircularQueue(k);
 * bool param_1 = obj->enQueue(value);
 * bool param_2 = obj->deQueue();
 * int param_3 = obj->Front();
 * int param_4 = obj->Rear();
 * bool param_5 = obj->isEmpty();
 * bool param_6 = obj->isFull();
 */
