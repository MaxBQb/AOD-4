#pragma once
#include "Pair.h"
#include "FileDataBase.h"
#include "SplayTree.h"

template <class E>
class EngineSplayTree: public FileDataBase<E>::Engine {
public:
    int find(E key) override {
        auto* tmp = container.find({key, 0});
        if (tmp == nullptr)
            return -1;
        return tmp->data.value;
    }

    bool insert(E key, int value) override {
        inserted_keys_count++;
        container.insert({key, value});
        return true;
    }

    bool remove(E key) override {
        return container.remove({key, 0});
    }

    void print(ostream& out) override {
        container.print(out);
    }

    void onLoad() override {
        if (inserted_keys_count)
            cout << "Average rotations count: "
                << container.getRotationsCount()/inserted_keys_count
                << endl;
    }

private:
    unsigned inserted_keys_count = 0;
    SplayTree<Pair<E, int>> container;
};
