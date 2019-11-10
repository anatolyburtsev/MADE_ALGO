//
// Created by Анатолий Бурцев on 2019-11-09.
//

#include <iostream>
#include <queue>

struct Node {
    int value = 0;
    Node *Left = nullptr;
    Node *Right = nullptr;

    Node(int _value) {
        value = _value;
        Left = nullptr;
        Right = nullptr;
    }
};

class Tree {
public:
    ~Tree();

    void Print() const;

    void Add(int value);

    void PrintBFS() const;

private:
    Node *root = nullptr;

    void delete_subtree(Node *node);

};

Tree::~Tree() {
    delete_subtree(root);
}

void Tree::delete_subtree(Node *node) {
    if (!node) return;
    delete_subtree(node->Left);
    delete_subtree(node->Right);
    delete node;
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