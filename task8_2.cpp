//
// Created by Анатолий Бурцев on 2019-11-16.
//

#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>
#include<cassert>
#include <vector>

class Hashtable {
public:
    explicit Hashtable(size_t initial_size);

    ~Hashtable();

    bool Has(const std::string &key) const;

    bool Add(const std::string &key);

    bool Remove(const std::string &key);

    void Print();

    int Hash(const std::string *key) const;

private:
    // value of "a" for hash function
    int HASH_A = 137;
    double LOAD_FACTOR = 0.74;
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


    int Hash2(const std::string *key) const;

    int findIndexByKeyForAdd(int hash, const std::string &key) const;

    bool hasKey(int hash, const std::string &key) const;

    int findElement(int hash, const std::string &key) const;

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

//int Hashtable::Hash(const std::string *key) const {
//    long A = 2654435769;
//    int hash = (key * A);
//    for (char i : *key) {
//        hash = (hash * HASH_A + i);
//    }
//    return hash;
//}

int Hashtable::Hash(const std::string *key) const {
    int hash = 0;
//    for (char i : *key) {
//        hash = (hash * HASH_A + i);
//    }
    const char *str = key->c_str();
    for (; *str != 0; ++str) {
        hash = (hash * HASH_A + *str);
    }
    return hash;
}


int Hashtable::Hash2(const std::string *key) const {
//    return 1;
    int hash = 5;
    const char* str = key->c_str();
    for (; *str != 0; ++str) {
        hash += *str;
    }
//    for (char i : *key) {
//        hash += i;
//    }
    return 2 * hash + 1;
}

int Hashtable::findIndexByKeyForAdd(int hash, const std::string &key) const {
    int m = table->size();
    int initial_position = ((hash % m) + m) % m;
    if (table->at(initial_position) == nullptr) {
        return initial_position;
    }
    int hash2 = Hash2(&key) % m;
    int position = initial_position;
    int position_to_put_new_element = -1;
    while (true) {
        HashTableNode *element = table->at(position);
        // found empty place for new element
        if (element == nullptr) {
            if (position_to_put_new_element < 0) position_to_put_new_element = position;
            break;
        }
        // found place for new element with deleted previous one
        if (element->isDeleted && position_to_put_new_element < 0) position_to_put_new_element = position;

        // found duplicate, fatal for Add
        if (element->key == key && !element->isDeleted) return -1;

        position = (position + hash2) % m;
    }
    return position_to_put_new_element;
}

bool Hashtable::hasKey(int hash, const std::string &key) const {
    int m = table->size();
    int initial_position = ((hash % m) + m) % m;
    if (table->at(initial_position) == nullptr) {
        return false;
    }
    int hash2 = Hash2(&key) % m;
    int position = initial_position;
    while (true) {
        HashTableNode *element = table->at(position);
        if (element == nullptr) {
            return false;
        }

        if (element->key == key && element->isDeleted) {
            return false;
        }

        if (element->key == key && !element->isDeleted) {
            return true;
        }

        position = (position + hash2) % m;
    }
}

int Hashtable::findElement(int hash, const std::string &key) const {
    int m = table->size();
    int initial_position = ((hash % m) + m) % m;
    if (table->at(initial_position) == nullptr) {
        return -1;
    }
    int hash2 = Hash2(&key) % m;
    int position = initial_position;
    while (true) {
        HashTableNode *element = table->at(position);
        if (element == nullptr) {
            return -1;
        }

        if (element->key == key && element->isDeleted) {
            return -1;
        }

        if (element->key == key && !element->isDeleted) {
            return position;
        }

        position = (position + hash2) % m;
    }
}


bool Hashtable::Add(const std::string &key) {
    int hash = Hash(&key);
    auto *node = new HashTableNode(key, hash);
    int index = findIndexByKeyForAdd(hash, key);
    if (index < 0) {
        return false;
    } else {
        table->at(index) = node;
        numberOfElements++;
        double load_factor = double(numberOfElements) / table->capacity();
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
            std::cout << table->at(i)->key << ":";
            if (table->at(i)->isDeleted) {
                std::cout << "-";
            } else {
                std::cout << "+";
            }
            std::cout << " ";
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
        if (node->isDeleted) continue;
        int index = findIndexByKeyForAdd(node->hash, node->key);
        if (index < 0) {
            throw std::runtime_error("grow command failed with negative index");
        }
        table->at(index) = node;
    }

    delete old_data;
}

bool Hashtable::Has(const std::string &key) const {
    int hash = Hash(&key);
    return hasKey(hash, key);
}

bool Hashtable::Remove(const std::string &key) {
    int hash = Hash(&key);
    int index = findElement(hash, key);
    if (index < 0) {
        return false;
    } else {
        HashTableNode *elem = table->at(index);
        numberOfElements--;
        elem->isDeleted = true;
        return true;
    }
}
#include <fstream>

int main() {
    Hashtable table(8);
    char operation;
    std::string value;
    std::ifstream inFile;
    inFile.open("/Users/onotole/CLionProjects/made_algo/input.txt");
    clock_t begin = clock();
    while(inFile >> operation >> value) {
//    while (std::cin >> operation >> value) {
        switch (operation) {
            case '+':
//                table.Add(value);
                std::cout << (table.Add(value) ? "OK" : "FAIL") << std::endl;
                break;
            case '-':
//                table.Remove(value);
                std::cout << (table.Remove(value) ? "OK" : "FAIL") << std::endl;
                break;
            case '?':
//                table.Has(value);
                std::cout << (table.Has(value) ? "OK" : "FAIL") << std::endl;
                break;
            default:
                assert(false);
        }
//        table.Print();
    }
    clock_t end = clock();
    std::cout << "Elapsed cycles: " << end - begin << std::endl;

//    std::cout << "result: " << table.Add("a") << std::endl;
//    std::cout << table.Remove("a") << std::endl;
//    std::cout << "result: " << table.Add("b") << std::endl;
//    std::cout << "result: " << table.Add("c") << std::endl;
//    std::cout << "result: " << table.Add("d") << std::endl;
//    table.Print();
//    std::cout << table.Has("a") << std::endl;
//    std::cout << "result: " << table.Add("Pew2") << std::endl;
//    std::cout << table.Has("at") << std::endl;
//    std::cout << "result: " << table.Add("ad") << std::endl;
//    table.Print();
//    table.Print();
//    table.Print();
//    table.Print();
    return 0;
}

