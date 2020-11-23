#pragma once
#include "Pair.h"
#include "FileDataBase.h"
#include "BinarySearchTree.h"

template <class E>
class EngineBinarySearchTree: public FileDataBase<E>::Engine {
public:
    int find(E key) override {
        auto* tmp = container.find({key, 0});
        if (tmp == nullptr)
            return -1;
        return tmp->data.value;
    }

    bool insert(E key, int value) override {
        container.insert({key, value});
        return true;
    }

    bool remove(E key) override {
        return container.remove({key, 0});
    }

    void print(ostream& out) {
        container.print(out);
    }

private:
    BinarySearchTree<Pair<E, int>> container;
};
