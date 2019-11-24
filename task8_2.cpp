//
// Created by Анатолий Бурцев on 2019-11-16.
//
//Реализуйте структуру данных типа “множество строк” на основе динамической хеш-таблицы с открытой адресацией.
// Хранимые строки непустые и состоят из строчных латинских букв. Хеш-функция строки должна быть реализована с
// помощью вычисления значения многочлена методом Горнера. Начальный размер таблицы должен быть равным 8-ми.
// Перехеширование выполняйте при добавлении элементов в случае, когда коэффициент заполнения таблицы достигает 3/4.
// Структура данных должна поддерживать операции добавления строки в множество, удаления строки из множества и проверки
// принадлежности данной строки множеству. 1_1. Для разрешения коллизий используйте квадратичное пробирование.
// i-ая проба g(k, i)=g(k, i-1) + i (mod m). m - степень двойки. 1_2. Для разрешения коллизий используйте
// двойное хеширование.
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

    int Hash(const std::string *key) const;

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
    table->clear();
    delete table;
}

int Hashtable::Hash(const std::string *key) const {
    int hash = 0;
    const char *str = key->c_str();
    for (; *str != 0; ++str) {
        hash = (hash * HASH_A + *str);
    }
    return hash;
}


int Hashtable::Hash2(const std::string *key) const {
    int hash = 5;
    const char* str = key->c_str();
    for (; *str != 0; ++str) {
        hash += *str;
    }
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

    old_data->clear();
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
        elem->isDeleted = true;
        return true;
    }
}

int main() {
    Hashtable table(8);
    char operation;
    std::string value;
    while (std::cin >> operation >> value) {
        switch (operation) {
            case '+':
                std::cout << (table.Add(value) ? "OK" : "FAIL") << std::endl;
                break;
            case '-':
                std::cout << (table.Remove(value) ? "OK" : "FAIL") << std::endl;
                break;
            case '?':
                std::cout << (table.Has(value) ? "OK" : "FAIL") << std::endl;
                break;
            default:
                assert(false);
        }
    }
    return 0;
}

