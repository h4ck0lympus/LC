#include <bits/stdc++.h>

using namespace std;

int __speed = [] {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    return 0;
}();

class LRUCache {
    struct Node {
        int key;
        int value;
        Node* next;
        Node* prev;

        Node(int key, int value)
            : key(key)
            , value(value)
            , next(nullptr)
            , prev(nullptr)
        {
        }
    };

private:
    Node* head;
    Node* tail;
    unordered_map<int, Node*> m;
    int capacity;

public:
    LRUCache(int capacity)
        : capacity(capacity)
    {
        // use dummy node for head and tail
        // dummy node simplify the handling of edge cases
        // edge cases where head and tail are involved
        head = new Node(-1, -1);
        tail = new Node(-1, -1);
        head->next = tail;
        tail->prev = head;
    }

    void unlink(Node* node)
    {
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }

    void putAtHead(Node* node)
    {
        Node* temp = head->next;
        head->next = node;
        node->prev = head;
        node->next = temp;
        temp->prev = node;
    }

    int get(int key)
    {
        if (m.find(key) != m.end()) {
            Node* node = m[key];
            unlink(node);
            putAtHead(node); // accesed recently
            return node->value;
        }
        return -1;
    }

    void put(int key, int value)
    {
        if (m.find(key) != m.end()) {
            // cache hit
            Node* node = m[key];
            node->value = value;
            unlink(node);
            putAtHead(node);
        } else {
            // cache miss
            Node* newNode = new Node(key, value);
            if (m.size() == capacity) {
                Node* toBeDeleted = tail->prev;
                m.erase(toBeDeleted->key);
                unlink(toBeDeleted);
                putAtHead(newNode);
                m[key] = newNode;
                delete toBeDeleted;
            } else {
                putAtHead(newNode);
                m[key] = newNode;
            }
        }
    }
};

int main(int argc, char* argv[])
{
    LRUCache* lru = new LRUCache(2);
    lru->put(1, 1);
    lru->put(2, 2);
    cout << lru->get(1) << endl;
    lru->put(3, 3);
    cout << lru->get(2) << endl;
    lru->put(4, 4);
    cout << lru->get(1) << endl;
    cout << lru->get(3) << endl;
    cout << lru->get(4) << endl;
    return 0;
}
