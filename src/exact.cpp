#include "exact.h"
#include <stdexcept>
#include <utility>

HashTable::HashTable(int buckets) : size(buckets > 0 ? buckets : 1), used(0) {
    table = new Node*[size]();
}
HashTable::~HashTable() {
    for (int i = 0; i < size; i++) {
        while (table[i]) {
            Node* old = table[i];
            table[i] = old->next;
            delete old;
        }
    }
    delete[] table;
}
int HashTable::hashIndex(const std::string& key) const {
    unsigned long hash = 0;
    for (unsigned char c : key) hash = hash * 31 + c;
    return hash % size;
}
int HashTable::hashUpdate(const std::string& key) {
    if (key.empty()) return 0;
    int index = hashIndex(key);
    Node* current = table[index];
    while (current) {
        if (current->key == key) return ++current->count;
        current = current->next;
    }
    table[index] = new Node{key, 1, table[index]};
    used++;
    return 1;
}
int HashTable::hashCount(const std::string& key) const {
    Node* current = table[hashIndex(key)];
    while (current) {
        if (current->key == key) return current->count;
        current = current->next;
    }
    return 0;
}
MinHeap::MinHeap(int k) : size(0), capacity(k > 0 ? k : 0) {
    heap = new Entry[capacity];
}
MinHeap::~MinHeap() { delete[] heap; }
bool MinHeap::worse(const Entry& a, const Entry& b) const {
    return a.count < b.count || (a.count == b.count && a.key > b.key);
}
void MinHeap::siftUp(int i) {
    while (i > 0) {
        int parent = (i - 1) / 2;
        if (!worse(heap[i], heap[parent])) break;
        std::swap(heap[i], heap[parent]);
        i = parent;
    }
}
void MinHeap::siftDown(int i) {
    while (2*i + 1 < size) {
        int child = 2*i + 1;
        if (child + 1 < size && worse(heap[child+1], heap[child])) child++;
        if (!worse(heap[child], heap[i])) break;
        std::swap(heap[child], heap[i]);
        i = child;
    }
}
void MinHeap::heapUpdateTopK(const std::string& key, int count) {
    if (capacity == 0) return;
    // Entries arrive once each from the exact table when results are requested.
    Entry item{key, count};
    if (size < capacity) {
        heap[size] = item;
        size++;
        siftUp(size - 1);
    } else if (worse(heap[0], item)) {
        heap[0] = item;
        siftDown(0);
    }
}
Entry MinHeap::removeMin() {
    if (size == 0) throw std::out_of_range("Empty heap");
    Entry result = heap[0];
    heap[0] = heap[--size];
    siftDown(0);
    return result;
}
void HashTable::topK(int k, Entry* output, int& length) const {
    length = 0;
    if (k <= 0 || used == 0) return;
    if (k > used) k = used;
    MinHeap heap(k);
    for (int i = 0; i < size; i++)
        for (Node* p = table[i]; p; p = p->next)
            heap.heapUpdateTopK(p->key, p->count);
    length = heap.length();
    for (int i = length - 1; i >= 0; i--) output[i] = heap.removeMin();
}
