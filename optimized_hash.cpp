#include <iostream>
#include <cstdlib>
#include <ctime>

const int TABLE_SIZE = 200000003; // A large prime number > 100 million for low collision rate
//const int TABLE_SIZE =   1000000;

class HashTable {
private:
    int* keys;
    int* values;
    bool* occupied;

    int hash(int key) {
        return key % TABLE_SIZE;
    }

public:
    HashTable() {
        keys = new int[TABLE_SIZE];
        values = new int[TABLE_SIZE];
        occupied = new bool[TABLE_SIZE]{false};

    }

    ~HashTable() {
        delete[] keys;
        delete[] values;
        delete[] occupied;
    }

    void insert(int key, int value) {
        int idx = hash(key);
        while (occupied[idx] && keys[idx] != key) {
            idx = (idx + 1) % TABLE_SIZE; // Linear probing
        }
        keys[idx] = key;
        values[idx] = value;
        occupied[idx] = true;
    }

    bool get(int key, int &value) {
        int idx = hash(key);
        int start = idx;
        while (occupied[idx]) {
            if (keys[idx] == key) {
                value = values[idx];
                return true;
            }
            idx = (idx + 1) % TABLE_SIZE;
            if (idx == start) break; // Full cycle
        }
        return false;
    }

    void update(int key, int newValue) {
        int idx = hash(key);
        int start = idx;
        while (occupied[idx]) {
            if (keys[idx] == key) {
                values[idx] = newValue;
                return;
            }
            idx = (idx + 1) % TABLE_SIZE;
            if (idx == start) break;
        }
        // Not found: do nothing (or could insert)
    }
};

int main() {
    std::cout << "Initializing hash table...\n";
    HashTable ht;

    const int NUM_INSERTS = 100000000;
    std::cout << "Inserting 100 million key-value pairs...\n";

    for (int i = 0; i < NUM_INSERTS; ++i) {
        ht.insert(i, i * 2); // value = 2 * key
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
