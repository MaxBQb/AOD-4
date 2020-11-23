#include <iostream>
#include "FileDataBase.h"
#include "DataBaseTester.h"
#include "EngineHashTable.h"
#include "EngineBinarySearchTree.h"
#include "EngineSplayTree.h"
#include "EntryPhoneOwner.h"
using namespace std;

int main() {
    auto *field_example = new EntryPhoneOwner();
    FileDataBase<int> db("phones.txt", nullptr, field_example);
    DataBaseTester<int> db_tester(db);


    cout << "Entries count: 10\n";
    db.generateFile(10);
    cout << "[HashTable Test]\n";
    db_tester.test(new EngineHashTable());
    cout << "\n\n[BinarySearchTree Test]\n";
    db_tester.test(new EngineBinarySearchTree<int>());
    cout << "\n\n[SplayTree Test]\n";
    db_tester.test(new EngineSplayTree<int>());

    db_tester.silentMode() = true;

    cout << "\n\nEntries count: 10^6\n";
    db.generateFile(1000000);
    cout << "[HashTable Test]\n";
    db_tester.test(new EngineHashTable());
    cout << "\n\n[BinarySearchTree Test]\n";
    db_tester.test(new EngineBinarySearchTree<int>());
    cout << "\n\n[SplayTree Test]\n";
    db_tester.test(new EngineSplayTree<int>());

    cout << "\n\nEntries count: 10^6, repeat 1000 times\n";
    cout << "[HashTable Test]\n";
    db_tester.test(new EngineHashTable(), 1000);
    cout << "\n\n[BinarySearchTree Test]\n";
    db_tester.test(new EngineBinarySearchTree<int>(), 1000);
    cout << "\n\n[SplayTree Test]\n";
    db_tester.test(new EngineSplayTree<int>(), 1000);

    delete field_example;
}



