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
        bool isDummy;

        // element was initialized only by default value
        bool isDummyElement() const { return isDummy; }

        HashTableNode(std::string key_, int hash_) : key(std::move(key_)), hash(hash_), isDeleted(false), isDummy(false) {}

        void update(std::string key_, int hash_) {
            key = std::move(key_);
            hash = hash_;
            isDeleted = false;
            isDummy = false;
        }

        HashTableNode() : key(""), hash(-1), isDeleted(true), isDummy(true) {}
    };


    std::vector<HashTableNode> table;

    int Hash(const std::string &key) const;

    int Hash2(const std::string &key) const;

    int findIndexByKeyForAdd(int hash, const std::string &key) const;

    int findElement(int hash, const std::string &key) const;

    void grow();
};

Hashtable::Hashtable(size_t initial_size) : table(initial_size), numberOfElements(0) {}

Hashtable::~Hashtable() {
}

int Hashtable::Hash(const std::string &key) const {
    int hash = 0;
    const char *str = key.c_str();
    for (; *str != 0; ++str) {
        hash = (hash * HASH_A + *str);
    }
    return hash;
}


int Hashtable::Hash2(const std::string &key) const {
    int hash = 5;
    const char *str = key.c_str();
    for (; *str != 0; ++str) {
        hash += *str;
    }
    return 2 * hash + 1;
}

int Hashtable::findIndexByKeyForAdd(int hash, const std::string &key) const {
    int m = table.size();
    int initial_position = ((hash % m) + m) % m;
    if (table.at(initial_position).isDummyElement()) {
        return initial_position;
    }
    int hash2 = Hash2(key) % m;
    int position = initial_position;
    int position_to_put_new_element = -1;
    while (true) {
        HashTableNode element = table.at(position);
        // found empty place for new element
        if (element.isDummyElement()) {
            if (position_to_put_new_element < 0) position_to_put_new_element = position;
            break;
        }
        // found place for new element with deleted previous one
        if (element.isDeleted && position_to_put_new_element < 0) position_to_put_new_element = position;

        // found duplicate, fatal for Add
        if (element.key == key && !element.isDeleted) return -1;

        position = (position + hash2) % m;
    }
    return position_to_put_new_element;
}

int Hashtable::findElement(int hash, const std::string &key) const {
    int m = table.size();
    int initial_position = ((hash % m) + m) % m;
    if (table.at(initial_position).isDummyElement()) {
        return -1;
    }
    int hash2 = Hash2(key) % m;
    int position = initial_position;
    while (true) {
        HashTableNode element = table.at(position);
        if (element.isDummyElement()) {
            return -1;
        }

        if (element.key == key && element.isDeleted) {
            return -1;
        }

        if (element.key == key && !element.isDeleted) {
            return position;
        }
        position = (position + hash2) % m;
    }
}


bool Hashtable::Add(const std::string &key) {
    int hash = Hash(key);
    int index = findIndexByKeyForAdd(hash, key);
    if (index < 0) {
        return false;
    }

    table.at(index).update(key, hash);
    numberOfElements++;
    double load_factor = double(numberOfElements) / table.capacity();
    if (load_factor >= LOAD_FACTOR) {
        grow();
    }
    return true;
}

void Hashtable::grow() {
    std::vector<HashTableNode> old_data = table;
    table = std::vector<HashTableNode>(2 * table.size());
    for (const HashTableNode& node: old_data) {
        if (node.isDummyElement()) continue;
        if (node.isDeleted) continue;
        int index = findIndexByKeyForAdd(node.hash, node.key);
        if (index < 0) {
            throw std::runtime_error("grow command failed with negative index");
        }
        table.at(index).update(node.key, node.hash);
    }
    // easy way to delete vector from memory based on
    // https://stackoverflow.com/questions/10464992/c-delete-vector-objects-free-memory
    std::vector<HashTableNode>().swap(old_data);
}

bool Hashtable::Has(const std::string &key) const {
    int hash = Hash(key);
    return findElement(hash, key) >= 0;
}

bool Hashtable::Remove(const std::string &key) {
    int hash = Hash(key);
    int index = findElement(hash, key);
    if (index < 0) {
        return false;
    }
    table.at(index).isDeleted = true;
    return true;

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

