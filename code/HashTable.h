#pragma once
#include <iostream>
#include <cmath>

using namespace std;

template <class T>
class HashTable {
private:
    struct Node {
        int key = {};
        T value = {};
        bool freeNode = true;
    };

    Node* data;
    static const int default_size = 17;
    double resize_index;
    int capacity;
    int count;
    int insertions_count;

    int findKey(int key);
    unsigned hash_1(unsigned key) { return key % capacity; }
    unsigned hash_2(unsigned key) { return (key % (capacity - 1)) + 1; }
    inline unsigned step(unsigned key, unsigned i);
    int nextPrimeNumber(int previous);

public:
    HashTable();
    ~HashTable() { delete[] data; }
    bool insert(int key, T value);
    bool contains(int key) { return findKey(key) != -1; }
    void resize();
    void rehash();
    bool remove(int key);
    int getCapacity() { return capacity; }
    int getCount() { return count; }
    T& operator[](int key);
    T* get(int key);

    template<class U>
    friend ostream& operator<<(ostream& os, HashTable<U>& H);
};

template<class T>
HashTable<T>::HashTable() {
    resize_index = 0.75;
    capacity = default_size;
    insertions_count = 0;
    count = 0;
    data = new Node[capacity];
}

template<class T>
bool HashTable<T>::insert(int key, T value) {
    if (count + 1 > int(resize_index * capacity))
        resize();
    else if (insertions_count > 2 * count)
        rehash();

    bool isInsertionSucceed = false;
    int i = 0;
    unsigned h;

    while (i < capacity && !isInsertionSucceed) {
        h = step(key, i);
        if (data[h].freeNode) {
            data[h].key = key;
            data[h].value = value;
            data[h].freeNode = false;
            ++insertions_count;
            ++count;
            isInsertionSucceed = true;
        } else if (data[h].key == key)
            return false;
        i++;
    }
    return isInsertionSucceed;
}

template<class T>
void HashTable<T>::resize() {
    int new_capacity = nextPrimeNumber(capacity*2);
    Node* arr2 = new Node[new_capacity];
    for (int i = 0; i < capacity; i++) {
        if (!data[i].freeNode) {
            arr2[i].key = data[i].key;
            arr2[i].value = data[i].value;
            arr2[i].freeNode = false;
        }
    }
    capacity = new_capacity;
    delete[] data;
    data = arr2;
}

template<class T>
void HashTable<T>::rehash() {
    Node* arr2 = new Node[capacity];
    for (int i = 0; i < capacity; i++) {
        if (!data[i].freeNode) {
            arr2[i].key = data[i].key;
            arr2[i].value = data[i].value;
            arr2[i].freeNode = false;
            data[i] = Node();
        }
    }
    insertions_count = 0;
    count = 0;
    for (int i = 0; i < capacity; i++)
        if (!arr2[i].freeNode)
            insert(arr2[i].key, arr2[i].value);
    delete[] arr2;
}

template<class T>
T &HashTable<T>::operator[](int key) {
    int position = findKey(key);
    if (position < 0)
        throw exception();
    return data[position].value;
}

template<class T>
T *HashTable<T>::get(int key)  {
    int position = findKey(key);
    if (position < 0)
        return nullptr;
    return &data[position].value;
}

template<class T>
bool HashTable<T>::remove(int key) {
    int position = findKey(key);
    if (position < 0)
        return false;
    data[position].freeNode = true;
    count--;
    return true;
}

template<class T>
unsigned HashTable<T>::step(unsigned key, unsigned i) {
    return (hash_1(key) + i * hash_2(key)) % capacity;
}

template<class T>
int HashTable<T>::findKey(int key) {
    unsigned h;
    for (int i = 0; i < capacity; i++) {
        h = step(key, i);
        if (data[h].freeNode)
            return -1;
        if (data[h].key == key)
            return h;
    }
    return -1;
}

template <class T>
ostream& operator<<(ostream& os, HashTable<T>& H) {
    for (int i = 0; i < H.getCapacity(); i++) {
        if (H.data[i].freeNode)
            os << i << " [---]: available\n";
        else os << i << " [" << H.data[i].key << "]: " << H.data[i].value << endl;
    }
    return os;
}

template<class T>
int HashTable<T>::nextPrimeNumber(int previous) {
    int temp = previous + 1;
    int next = 0;
    for (int i = 2;; i++) {
        if (!(temp%i) && i != temp) {
            temp++;
            i=2;
            continue;
        }
        if ((i==temp) || (i>sqrt(temp))) {
            next = temp;
            break;
        }
    }

    return next;
}
