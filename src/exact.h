#ifndef EXACT_H
#define EXACT_H
#include <string>
struct Entry { std::string key; int count; };
class HashTable {
    struct Node { std::string key; int count; Node* next; };
    Node** table;
    int size, used;
    int hashIndex(const std::string& key) const;
public:
    explicit HashTable(int buckets = 1024);
    ~HashTable();
    HashTable(const HashTable&) = delete;
    HashTable& operator=(const HashTable&) = delete;
    int hashUpdate(const std::string& key);
    int hashCount(const std::string& key) const;
    int uniqueCount() const { return used; }
    void topK(int k, Entry* output, int& length) const;
};
class MinHeap {
    Entry* heap;
    int size, capacity;
    bool worse(const Entry& a, const Entry& b) const;
    void siftUp(int i);
    void siftDown(int i);
public:
    explicit MinHeap(int k);
    ~MinHeap();
    MinHeap(const MinHeap&) = delete;
    MinHeap& operator=(const MinHeap&) = delete;
    void heapUpdateTopK(const std::string& key, int count);
    Entry removeMin();
    int length() const { return size; }
};
#endif
