#pragma once
#include <iostream>
#include "FileDataBase.h"
#include "Watch.h"
using namespace std;

template <class T>
class DataBaseTester {
    struct OUTPUT {
        // Выключаемый вывод
        bool silent_mode = false;

        template <class E>
        OUTPUT& operator <<(const E& element) {
            if (!silent_mode)
                cout << element;
            return *this;
        }
    } out;

    Watch timer;
    FileDataBase<T>& db;

    template<class M = chrono::nanoseconds>
    inline void time_show(string units = "ns") {
        cout << " --- Time estimate: " << (unsigned)(timer.getSessionLength<M>()/timer.getIntervalsCount()) << units << " ---\n";
    }

    T generateKey(bool existing);

public:
    DataBaseTester(FileDataBase<T> &db): db(db) {}

    void test(typename FileDataBase<T>::Engine* engine = nullptr, unsigned repeat_times = 1);
    void testSearching(T key);
    void testInsertion(T key);
    void testRemoving(T key);

    // Сквозной доступ к полю private структуры, не уверен, что хорошо так делать.
    bool& silentMode() { return out.silent_mode; }
};

template<class T>
void DataBaseTester<T>::test(typename FileDataBase<T>::Engine *engine, unsigned repeat_times) {
    if (engine != nullptr)
        db.setEngine(engine);

    cout << "Searching test:\n";

    timer.startSession();
    for (unsigned i = 0; i < repeat_times; i++)
        testSearching(generateKey(true));
    time_show();

    timer.startSession();
    for (unsigned i = 0; i < repeat_times; i++)
        testSearching(generateKey(false));
    time_show();


    cout << "\nRemoving test:\n";
    timer.startSession();
    for (unsigned i = 0; i < repeat_times; i++)
        testRemoving(generateKey(true));
    time_show();

    timer.startSession();
    for (unsigned i = 0; i < repeat_times; i++)
        testRemoving(generateKey(false));
    time_show();


    cout << "\nInsertion test:\n";
    timer.startSession();
    for (unsigned i = 0; i < repeat_times; i++)
        testInsertion(generateKey(true));
    time_show();

    timer.startSession();
    for (unsigned i = 0; i < repeat_times; i++)
        testInsertion(generateKey(false));
    time_show();

    out << "\nFinal structure:\n";
    if (!out.silent_mode)
        db.showStructure();

    delete engine; // опять же не уверен, что хорошо так делать
}

template<class T>
void DataBaseTester<T>::testSearching(T key) {
    timer.start();
    int search_result = db.getEngine()->find(key);
    timer.stop();
    out << "Attempt to find key: " << key << '\n';
    if (search_result == -1) {
        out << "Entries not found!\n";
        return;
    }
    out << "Result: ";
    if (!out.silent_mode)
        db.find(key)->print(cout);
    out << '\n';
}

template<class T>
void DataBaseTester<T>::testInsertion(T key) {
    timer.start();
    bool insertion_success = db.getEngine()->insert(key, 0);
    timer.stop();

    out << "Attempt to insert key: " << key << '\n';
    if (!insertion_success) {
        out << "Insertion aborted!\n";
        return;
    }
    out << "Attempt to proof insertion: ";
    int search_result = db.getEngine()->find(key);
    if (search_result == -1)
        // Если что-то идёт не так об этом точно стоит знать
        cout << "ERROR (inserted key NOT found!)\n";
    else
        out << "SUCCESS (inserted key found)\n";
}

template<class T>
void DataBaseTester<T>::testRemoving(T key) {
    timer.start();
    bool removing_success = db.getEngine()->remove(key);
    timer.stop();

    out << "Attempt to remove key: " << key << '\n';
    if (!removing_success) {
        out << "Nothing changed.\n";
        return;
    }
    out << "Attempt to proof removing: ";
    int search_result = db.getEngine()->find(key);
    if (search_result == -1)
        out << "SUCCESS (key not found)\n";
    else
        out << "WARNING (key found, clone maybe?)\n";
}

template<class T>
T DataBaseTester<T>::generateKey(bool existing) {
    if (existing) {
        auto &keys = db.getKeys();
        if (keys.empty())
            return {};
        return keys[rand()%keys.size()];
    }

    T value;
    do { value = db.getCurrentEntry()->generate(); }
    while (db.getEngine()->find(value) != -1);
    return value;
}

