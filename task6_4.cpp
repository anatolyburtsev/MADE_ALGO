//
// Created by Анатолий Бурцев on 2019-11-09.
// Задача 6. Порядок обхода (3 балла)
// Дано число N < 106 и последовательность целых чисел из [-2^31..2^31] длиной N.
// Требуется построить бинарное дерево, заданное наивным порядком вставки.
// Т.е., при добавлении очередного числа K в дерево с корнем root, если root→Key ≤ K, то узел K добавляется в правое поддерево root; иначе в левое поддерево root.
// Рекурсия запрещена.
// 6_4. Выведите элементы в порядке level-order (по слоям, “в ширину”).


#include <iostream>
#include <queue>

struct Node {
    int value = 0;
    Node *Left = nullptr;
    Node *Right = nullptr;

    explicit Node(int _value) : value(_value) {}
};

class Tree {
public:
    ~Tree();

    void Add(int value);

    void PrintBFS() const;

private:
    Node *root = nullptr;
};

Tree::~Tree() {
    std::queue<Node *> queue;
    queue.push(root);
    while(!queue.empty()) {
       Node *nodeToDelete = queue.front();
       queue.pop();
       if (nodeToDelete) {
           queue.push(nodeToDelete->Right);
           queue.push(nodeToDelete->Left);
           delete nodeToDelete;
       }
    }
}

void Tree::Add(int value) {
    Node *newNode = new Node(value);
    Node *node = root;
    if (!node) {
        root = newNode;
        return;
    }

    while (true) {
        if (node->value <= value) {
            // go right
            if (node->Right) {
                node = node->Right;
                continue;
            } else {
                node->Right = newNode;
                break;
            }
        } else {
            if (node->Left) {
                node = node->Left;
                continue;
            } else {
                node->Left = newNode;
                break;
            }
        }
    }
}

void Tree::PrintBFS() const {
    std::queue<Node *> queue;
    queue.push(root);
    while (!queue.empty()) {
        Node *node = queue.front();
        queue.pop();
        if (node != root)
            std::cout << " ";
        std::cout << node->value;
        if (node->Left)
            queue.push(node->Left);
        if (node->Right)
            queue.push(node->Right);
    }

}

int main() {
    Tree tree;
    int n;
    int tmp;
    std::cin >> n;
    for (int i = 0; i < n; ++i) {
        std::cin >> tmp;
        tree.Add(tmp);
    }
    tree.PrintBFS();
    return 0;
}
