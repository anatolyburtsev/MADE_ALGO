//
// Created by Анатолий Бурцев on 2019-11-16.
//

#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

class Hashtable {
public:
    explicit Hashtable(size_t initial_size);

    ~Hashtable();


    bool Has(const std::string &key) const;

    bool Add(const std::string &key);

    bool Remove(const std::string &key);

    void Print();


private:
    // value of "a" for hash function
    int HASH_A = 137;
    double LOAD_FACTOR = 0.75;
    int numberOfElements;

    struct HashTableNode {
        int hash;
        std::string key;
        bool isDeleted;

        HashTableNode(std::string key_, int hash_) {
            this->key = std::move(key_);
            this->hash = hash_;
            this->isDeleted = false;
        }
    };

    std::vector<HashTableNode *> *table;

    int Hash(const std::string *key);

    int Hash2(const std::string *key);

    int getIndexForInsert(int hash, std::string key);

    void grow();
};

Hashtable::Hashtable(size_t initial_size) {
    table = new std::vector<HashTableNode *>(initial_size);
    numberOfElements = 0;
}

Hashtable::~Hashtable() {
    for (HashTableNode *node : *table) {
        delete node;
    }
    delete table;
}

int Hashtable::Hash(const std::string *key) {
    int hash = 0;
    for (char i : *key) {
        hash = (hash * HASH_A + i);
    }
    return hash;
}

int Hashtable::Hash2(const std::string *key) {
    int hash = 0;
    for (char i : *key) {
        hash += i;
    }
    return 2 * hash + 1;
}

int Hashtable::getIndexForInsert(int hash, std::string key) {
    int m = table->size();
    int initial_position = hash % m;
    if (!table->at(initial_position) || table->at(initial_position)->isDeleted) {
        return initial_position;
    }
    int hash2 = Hash2(&key);
    int position = initial_position;
    while (true) {
        // found place for new element
        if (!table->at(position) || table->at(position)->isDeleted) break;

        // found duplicate
        if (table->at(position)->key == key) return -1;

        position = (position + hash2) % m;
    }
    return position;
}

bool Hashtable::Add(const std::string &key) {
    int hash = Hash(&key);
    auto *node = new HashTableNode(key, hash);
    int index = getIndexForInsert(hash, key);
    std::cout << "index: " << index << std::endl;
    if (index < 0) {
        return false;
    } else {
        table->at(index) = node;
        numberOfElements++;
        double load_factor = double(numberOfElements) / table->capacity();
//        std::cout << "load factor " << load_factor << std::endl;
        if (load_factor >= LOAD_FACTOR) {
            grow();
        }
        return true;
    }
}

void Hashtable::Print() {
    std::cout << std::endl;
    for (int i = 0; i < table->size(); ++i) {
        if (table->at(i)) {
            std::cout << table->at(i)->key << " ";
        } else {
            std::cout << "null" << " ";
        }
    }
    std::cout << std::endl;
}

void Hashtable::grow() {
    std::vector<HashTableNode *> *old_data = table;
    table = new std::vector<HashTableNode *>(2 * table->size());
    for (HashTableNode *node: *old_data) {
        if (!node) continue;
        int index = getIndexForInsert(node->hash, node->key);
        if (index < 0) {
            throw std::runtime_error("grow command failed with negative index");
        }
        table->at(index) = node;
    }

    delete old_data;
}

int main() {
    Hashtable table(4);
    table.Print();
    std::cout << "result: " << table.Add("ad") << std::endl;
    table.Print();
    std::cout << "result: " << table.Add("bc") << std::endl;
    table.Print();
    std::cout << "result: " << table.Add("cb") << std::endl;
//    std::cout << "result: " << table.Add("Pew2") << std::endl;
//    table.Print();
    table.Print();
    return 0;
}

