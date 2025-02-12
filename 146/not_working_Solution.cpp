#include <bits/stdc++.h>

#ifdef __cplusplus
extern "C"
#endif

    const char*
    __asan_default_options()
{
    return "detect_leaks=0";
}

using namespace std;

class LRUCache {
    struct Node {
        int key, value;
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

    int capacity;
    Node* cacheHead;
    Node* cacheTail;
    map<int, Node*> m; // Map to store key-node pairs
    int size;

public:
    LRUCache(int capacity)
        : capacity(capacity)
        , size(0)
        , cacheHead(nullptr)
        , cacheTail(nullptr)
    {
        for (int i = 0; i < capacity; i++) {
            m[i] = nullptr;
        }
    }

    int get(int key)
    {
        if (m.find(key) == m.end() || m[key] == nullptr) {
            return -1;
        }
        Node* node = m[key];
        putToFront(node); // most recently used/retrieved
        return node->value;
    }

    void put(int key, int value)
    {
        if (m.find(key) != m.end() && m[key] != nullptr) {
            // Cache hit
            Node* node = m[key];
            node->value = value;
            putToFront(node);
        } else {
            // Cache miss, add a new node
            if (size == capacity) {
                Node* toBeDeleted = cacheTail;
                if (toBeDeleted) {
                    m[toBeDeleted->key] = nullptr;
                    cacheTail = toBeDeleted->prev;
                    if (cacheTail) {
                        cacheTail->next = nullptr;
                    }
                    size--;
                }
            }
            addNode(key, value);
            m[key] = cacheHead;
            size++;
        }
    }

private:
    void putToFront(Node* node)
    {
        if (node == cacheHead)
            return; // already at the head

        if (node == cacheTail) {
            cacheTail = node->prev;
            if (cacheTail)
                cacheTail->next = nullptr;
        } else {
            if (node->prev)
                node->prev->next = node->next;
            if (node->next)
                node->next->prev = node->prev;
        }

        node->next = cacheHead;
        node->prev = nullptr;
        if (cacheHead)
            cacheHead->prev = node;
        cacheHead = node;

        if (cacheTail == nullptr) {
            cacheTail = cacheHead; // If the list was empty
        }
    }

    // new node front of the cache
    void addNode(int key, int value)
    {
        Node* newNode = new Node(key, value);
        if (cacheHead == nullptr) {
            cacheHead = cacheTail = newNode; // First node in the cache
        } else {
            newNode->next = cacheHead;
            cacheHead->prev = newNode;
            cacheHead = newNode;
        }
    }
};

int main(int argc, char* argv[])
{
    LRUCache lru = *new LRUCache(1);
    cout << lru.get(6) << endl;
    cout << lru.get(8) << endl;
    lru.put(12, 1);
    cout << lru.get(2) << endl;
    lru.put(15, 11);
    lru.put(5, 2);
    lru.put(1, 15);
    lru.put(4, 2);
    cout << lru.get(5) << endl;
    lru.put(15, 15);
    return 0;
}
