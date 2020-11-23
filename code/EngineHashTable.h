#pragma once
#include "FileDataBase.h"
#include "HashTable.h"

class EngineHashTable: public FileDataBase<int>::Engine {
public:
    int find(int key) override {
        int* tmp = container.get(key);
        if (tmp == nullptr)
            return -1;
        return *tmp;
    }

    bool insert(int key, int value) override {
        return container.insert(key, value);
    }

    bool remove(int key) override {
        return container.remove(key);
    }

    virtual void print(ostream& out) {
        out << container;
    }
private:
    HashTable<int> container;
};
