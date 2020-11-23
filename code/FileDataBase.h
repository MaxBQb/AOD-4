#pragma once
#include <string>
#include <set>
#include <vector>
#include <fstream>
using namespace std;

template <class E>
class FileDataBase {
public:
    struct Entry {
        // Поле данных
        virtual E read(istream& fin) = 0;
        virtual void print(ostream& fout) const = 0;
        virtual E generate() = 0; // Создать случайное наполнение
    };

    struct Engine {
        // Система отвечающая за хранение/доступ/изменение пар ключ:смещение
        virtual int find(E key) = 0;
        virtual bool insert(E key, int value) = 0;
        virtual bool remove(E key) = 0;
        virtual void print(ostream& out) = 0;
        virtual void onLoad() {}; // Событие завершения загрузки
    };

    FileDataBase(string path, Engine *engine, Entry *field_example):
        path(path), engine(engine), current_entry(field_example) {}

    void loadFile();
    void showStructure();
    const Entry* find(E key);
    void generateFile(unsigned lines_count);
    void setEngine(Engine *engine);
    Engine* getEngine() { return engine; }
    const vector<E>& getKeys() const { return keys; }
    Entry* getCurrentEntry() { return current_entry; }

private:
    Engine *engine;
    Entry *current_entry;
    string path;

    vector<E> keys; // Just for testers
};

template<class E>
void FileDataBase<E>::loadFile() {
    if (engine == nullptr)
        return;
    ifstream fin(path, ios::binary);
    E key;
    int pos;
    if (fin.is_open())
        while (!fin.eof()) {
           pos = fin.tellg();
           key = current_entry->read(fin);
           if (!engine->insert(key, pos))
               cout << "ERROR\n";
           else
               keys.push_back(key);
        }
    fin.close();
    engine->onLoad();
}

template<class E>
void FileDataBase<E>::generateFile(unsigned int lines_count) {
    ofstream fout(path, ios::binary);
    set<E> entries;
    E value = {};
    if (fout.is_open())
        for (unsigned i = 0; i < lines_count; i++) {
            do { value = current_entry->generate(); }
            while (entries.find(value) != entries.end());
            entries.insert(value);
            current_entry->print(fout);
            if (i != lines_count-1)
                fout << '\n';
        }
    fout.close();
}

template<class E>
void FileDataBase<E>::setEngine(FileDataBase::Engine *engine) {
    this->engine = engine;
    loadFile();
}

template<class E>
const typename FileDataBase<E>::Entry *FileDataBase<E>::find(E key) {
    if (engine == nullptr)
        return nullptr;
    int res = engine->find(key);
    if (res == -1)
        return nullptr;
    ifstream fin(path, ios::binary);
    if (!fin.is_open())
        return nullptr;
    fin.seekg(res);
    current_entry->read(fin);
    fin.close();
    return current_entry;
}

template<class E>
void FileDataBase<E>::showStructure() {
    if (engine == nullptr)
        return;
    engine->print(cout);
}
