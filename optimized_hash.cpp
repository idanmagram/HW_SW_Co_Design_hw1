#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

const unsigned int TABLE_SIZE = 1 << 28;  // 268,435,456 slots
const unsigned int MASK = TABLE_SIZE - 1;
const unsigned int EMPTY_KEY = 0xFFFFFFFF; // A key that will never be used

struct HashEntry {
    unsigned int key;
    int value;
};

class HashTable {
private:
    HashEntry* table;

public:
    HashTable() {
        table = new HashEntry[TABLE_SIZE];
        for (unsigned int i = 0; i < TABLE_SIZE; ++i) {
            table[i].key = EMPTY_KEY;  // mark as empty
        }
    }

    ~HashTable() {
        delete[] table;
    }

    inline unsigned int hashFunction(unsigned int key) const {
        return key & MASK;
    }

    void insert(unsigned int key, int value) {
        unsigned int idx = hashFunction(key);
        while (table[idx].key != EMPTY_KEY && table[idx].key != key) {
            idx = (idx + 1) & MASK;
        }
        table[idx].key = key;
        table[idx].value = value;
    }

    bool get(unsigned int key, int& value_out) const {
        unsigned int idx = hashFunction(key);
        unsigned int start = idx;
        while (table[idx].key != EMPTY_KEY) {
            if (table[idx].key == key) {
                value_out = table[idx].value;
                return true;
            }
            idx = (idx + 1) & MASK;
            if (idx == start) break; // full cycle
        }
        return false;
    }

    bool update(unsigned int key, int new_value) {
        unsigned int idx = hashFunction(key);
        unsigned int start = idx;
        while (table[idx].key != EMPTY_KEY) {
            if (table[idx].key == key) {
                table[idx].value = new_value;
                return true;
            }
            idx = (idx + 1) & MASK;
            if (idx == start) break;
        }
        return false;
    }
};

int main() {
    HashTable ht;

    const unsigned int NUM_INSERTS = 100000000;

    cout << "Inserting 100 million key-value pairs...\n";
    for (unsigned int i = 0; i < NUM_INSERTS; ++i) {
        ht.insert(i, i << 1); // faster than i * 2 or i * 3
        if ((i & ((1 << 24) - 1)) == 0) {  // every ~16 million
            cout << (i / 1000000) << "M inserted...\n";
        }
    }
    cout << "Insertion complete.\n";

    cout << "Reading & updating 10 random keys...\n";
    srand(time(NULL));
    for (int i = 0; i < 10000; ++i) {
        unsigned int key = rand() % NUM_INSERTS;
        int value;
        if (ht.get(key, value)) {
            cout << "Key = " << key << ", Value = " << value << "\n";
            ht.update(key, value + 5);
            ht.get(key, value);
            cout << "After update: " << value << "\n";
        } else {
            cout << "Key not found.\n";
        }
    }

    return 0;
}
