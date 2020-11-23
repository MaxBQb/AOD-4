#pragma once

template <class K, class V>
struct Pair {
    K key;
    V value;
    bool operator>(const Pair &rhs) const { return key > rhs.key; }
    bool operator<(const Pair &rhs) const { return key < rhs.key; }
    bool operator>=(const Pair &rhs) const { return key >= rhs.key; }
    bool operator<=(const Pair &rhs) const { return key <= rhs.key; }
    bool operator==(const Pair &rhs) const { return key == rhs.key; }
    bool operator!=(const Pair &rhs) const { return key != rhs.key; }
    Pair(K key, V value):
        key(key), value(value) {}
    template<class K2, class V2>
    friend ostream& operator<<(ostream& out, Pair<K2,V2>& P);
};

template<class K, class V>
ostream &operator<<(ostream &out, Pair<K, V> &P) {
    return out << '<' << P.key << '|' << P.value << '>';
}

