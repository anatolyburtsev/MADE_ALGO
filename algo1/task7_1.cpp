//
// Created by Анатолий Бурцев on 2019-11-19.
//
#include <iostream>
#include <queue>

struct TreeNode {
    explicit TreeNode(int _value, int _priority) {
        Key = _value;
        Priority = _priority;
    }

    int Key = 0;
    int Priority = 0;
    TreeNode *Left = nullptr;
    TreeNode *Right = nullptr;
};

class Tree {
public:
    int Add(int key);

    void DeleteByKey(int key);

    void Delete(int number);
    ~Tree();
private:

    static void Split(TreeNode *node, int key, TreeNode *&left, TreeNode *&right) {
        if (!node) {
            left = right = nullptr;
        } else if (node->Key <= key) {
            Split(node->Right, key, node->Right, right);
            left = node;
        } else {
            Split(node->Left, key, left, node->Left);
            right = node;
        }
    }

    static TreeNode *Merge(TreeNode *left, TreeNode *right) {
        if (!left || !right)
            return left == nullptr ? right : left;
        if (left->Priority > right->Priority) {
            left->Right = Merge(left->Right, right);
            return left;
        } else {
            right->Left = Merge(left, right->Left);
            return right;
        }
    }

    TreeNode *root = nullptr;

    void fix_count(TreeNode *pNode, int key);

    int get_priority(TreeNode *pNode);

    int getWhereToStandPosition(int key);

    int find_k_statistic(int number, TreeNode *pNode);

    void delete_subtree(TreeNode *node);
};

Tree::~Tree() {
   delete_subtree(root);
}

void Tree::delete_subtree(TreeNode* node) {
    if (!node) return;
    delete_subtree(node->Left);
    delete_subtree(node->Right);
    delete node;
}

int Tree::Add(int key) {
    if (!root) {
        root = new TreeNode(key, 1);
        return 0;
    }
    TreeNode *left;
    TreeNode *right;
    Split(root, key, left, right);
    int unknown_priority = 0;
    TreeNode *newNode = new TreeNode(key, unknown_priority);
    if (left) {
        newNode = Merge(left, newNode);
    }
    if (right) {
        newNode = Merge(newNode, right);
    }
    root = newNode;
    fix_count(root, key);
    int whereToStandPosition = getWhereToStandPosition(key);
    return whereToStandPosition;
}

void Tree::fix_count(TreeNode *pNode, int key) {
    if (!pNode) return;
    if (pNode->Key > key) {
        fix_count(pNode->Left, key);
    } else if (pNode->Key < key) {
        // pNode-> Key < key => goes right
        fix_count(pNode->Right, key);
    }
    pNode->Priority = get_priority(pNode->Left) + get_priority(pNode->Right) + 1;
}

int Tree::get_priority(TreeNode *pNode) {
    if (!pNode) {
        return 0;
    } else {
        return pNode->Priority;
    }
}

int Tree::getWhereToStandPosition(int key) {
    TreeNode node = *root;
    int count = 0;
    while (node.Key != key) {
        if (node.Key > key) {
            count += 1 + get_priority(node.Right);
            node = *node.Left;
        } else {
            node = *node.Right;
        }
    }
    count += get_priority(node.Right);
    return count;
}

void Tree::DeleteByKey(int key) {
    if (root->Priority == 1 and root->Key == key) {
        root = nullptr;
        return;
    }
    TreeNode *left;
    TreeNode *right;
    Split(root, key, left, right);

    TreeNode *leftLeft;
    TreeNode *leftRight;
    Split(left, key - 1, leftLeft, leftRight);
    delete leftRight;

    root = Merge(leftLeft, right);

    fix_count(root, key);
}

void Tree::Delete(int number) {
    int key = find_k_statistic(number, root);
    DeleteByKey(key);
}

int Tree::find_k_statistic(int number, TreeNode *pNode) {
    if (get_priority(pNode->Right) == number) {
        return pNode->Key;
    }

    if (get_priority(pNode->Right) < number) {
        return find_k_statistic(number - get_priority(pNode->Right) - 1, pNode->Left);
    }

    if (get_priority(pNode->Right) > number) {
        return find_k_statistic(number, pNode->Right);
    }
}


int main() {
    Tree t;
    int n;
    int operation;
    int value;
    std::cin >> n;
    for (int i = 0; i < n; ++i) {
        std::cin >> operation;
        std::cin >> value;
        if (operation == 1) {
            std::cout << t.Add(value) << std::endl;
        } else {
            t.Delete(value);
        }
    }
    return 0;
}
