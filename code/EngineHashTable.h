#pragma once
#include "FileDataBase.h"
#include "HashTable.h"

/* Так как моя реализация хеш-таблицы может работать только
 * с ключами представленными int, то и доступен такой Engine
 * только для файловой бд работающей с int */
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
