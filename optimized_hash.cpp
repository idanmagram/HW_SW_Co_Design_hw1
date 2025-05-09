#include <iostream>
#include <cstdlib>
#include <ctime>

const int TABLE_SIZE = 200000003; // Large prime number > 100 million

struct HashEntry {
    int key;
    int value;
    bool occupied;
};

class HashTable {
private:
    HashEntry* table;

    int hash(int key) const {
        return key % TABLE_SIZE;
    }

public:
    HashTable() {
        table = new HashEntry[TABLE_SIZE];
        for (int i = 0; i < TABLE_SIZE; ++i)
            table[i].occupied = false;
    }

    ~HashTable() {
        delete[] table;
    }

    void insert(int key, int value) {
        int idx = hash(key);
        while (table[idx].occupied && table[idx].key != key) {
            idx = (idx + 1) % TABLE_SIZE;
        }
        table[idx].key = key;
        table[idx].value = value;
        table[idx].occupied = true;
    }

    bool get(int key, int& value) const {
        int idx = hash(key);
        int start = idx;
        while (table[idx].occupied) {
            if (table[idx].key == key) {
                value = table[idx].value;
                return true;
            }
            idx = (idx + 1) % TABLE_SIZE;
            if (idx == start) break;
        }
        return false;
    }

    void update(int key, int newValue) {
        int idx = hash(key);
        int start = idx;
        while (table[idx].occupied) {
            if (table[idx].key == key) {
                table[idx].value = newValue;
                return;
            }
            idx = (idx + 1) % TABLE_SIZE;
            if (idx == start) break;
        }
    }
};

int main() {
    std::cout << "Initializing hash table...\n";
    HashTable ht;

    const int NUM_INSERTS = 100000000;
    std::cout << "Inserting 100 million key-value pairs...\n";

    for (int i = 0; i < NUM_INSERTS; ++i) {
        ht.insert(i, i * 2);
        if (i % 10000000 == 0) std::cout << i / 1000000 << " million inserted...\n";
    }

    std::cout << "Insertions done.\n";

    std::cout << "Performing some read/write operations...\n";
    for (int i = 0; i < 10000; ++i) {
        int key = rand() % NUM_INSERTS;
        int val;
        if (ht.get(key, val)) {
            std::cout << "Key " << key << " has value " << val << "\n";
            ht.update(key, val + 1);
            ht.get(key, val);
            std::cout << "Updated value: " << val << "\n";
        } else {
            std::cout << "Key " << key << " not found!\n";
        }
    }

    return 0;
}
