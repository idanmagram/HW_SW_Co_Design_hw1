#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

const unsigned int TABLE_SIZE = 1 << 28;  // ~268 million entries, power of 2
const unsigned int MASK = TABLE_SIZE - 1;

struct HashEntry {
    unsigned int key;
    int value;
};

class HashTable {
private:
    HashEntry* table;
    char* status; // 0 = empty, 1 = filled

public:
    HashTable() {
        table = new HashEntry[TABLE_SIZE];
        status = new char[TABLE_SIZE];
        for (unsigned int i = 0; i < TABLE_SIZE; i++) {
            status[i] = 0; // mark all slots empty
        }
    }

    ~HashTable() {
        delete[] table;
        delete[] status;
    }

    inline unsigned int hashFunction(unsigned int key) {
        return key & MASK; // since TABLE_SIZE is power of 2
    }

    void insert(unsigned int key, int value) {
        unsigned int idx = hashFunction(key);
        while (status[idx] && table[idx].key != key) {
            idx = (idx + 1) & MASK;
        }
        table[idx].key = key;
        table[idx].value = value;
        status[idx] = 1;
    }

    bool get(unsigned int key, int& value_out) {
        unsigned int idx = hashFunction(key);
        unsigned int start = idx;
        while (status[idx]) {
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
        while (status[idx]) {
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
        ht.insert(i, i * 3);
        if (i % 10000000 == 0) {
            cout << i / 1000000 << "M inserted...\n";
        }
    }
    cout << "Insertion complete.\n";

    cout << "Reading & updating 10 random keys...\n";
    srand(time(NULL));
    for (int i = 0; i < 100000; ++i) {
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
