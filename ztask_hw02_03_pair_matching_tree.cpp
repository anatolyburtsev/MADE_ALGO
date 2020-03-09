#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdint>
#include <assert.h>
#include <unordered_map>
#include <string>

using namespace std;
//
// Created by Anatolii on 2020-03-06.
//


class Graph {
public:
    Graph(int n) {
        this->n = n;
        this->visited = new vector<int>(n);
        this->nodes = new vector<vector<int>>(n);
        this->answer_node_included = new vector<int>(n);
        this->answer_node_not_included = new vector<int>(n);
    }

    ~Graph() {
        this->visited->clear();
        this->answer_node_included->clear();
        this->answer_node_not_included->clear();
    }

    void addNode(int node, int parent) {
        if (parent < 0) {
            root = node;
        } else {
            nodes->at(parent).push_back(node);
        }
    }

    vector<int> getChilds(int u);

    int findMaxPairsTree(int root);

    int findMaxPairs();

private:
    int n;
    int root;
    vector<vector<int>> *nodes;
    vector<int> *visited;
    vector<int> *answer_node_included;
    vector<int> *answer_node_not_included;

};

vector<int> Graph::getChilds(int u) {
    return nodes->at(u);
}

int Graph::findMaxPairs() {
    return findMaxPairsTree(root);
}

int Graph::findMaxPairsTree(int root) {
    visited->at(root) = 1;
    vector<int> siblings = getChilds(root);
    vector<int> childs;
    // leaf
    if (siblings.empty()) {
        answer_node_included->at(root) = 0;
        answer_node_not_included->at(root) = -1;
    }
    int total = 0;
    for (int u: siblings) {
        if (visited->at(u) == 0) {
            childs.push_back(u);
            findMaxPairsTree(u);
            total += max(answer_node_included->at(u), answer_node_not_included->at(u));
        }
    }

    answer_node_not_included->at(root) = total;
    int total_node_included = -1;
    for (int u: childs) {
        int total_if_cur_node_included = total
                                         - max(answer_node_included->at(u), answer_node_not_included->at(u))
                                         + answer_node_not_included->at(u) + 1;
        if (total_if_cur_node_included > total_node_included) {
            total_node_included = total_if_cur_node_included;
        }
    }
    answer_node_included->at(root) = total_node_included;

    visited->at(root) = 2;
    return max(answer_node_included->at(root), answer_node_not_included->at(root));
}

int getResult(int n, int *nodes) {
    Graph g(n);
    for (int i = 0; i < n; ++i) {
        g.addNode(i, nodes[i]);
    }
    return n - g.findMaxPairs();
}

void test() {
    assert(getResult(3, new int[3]{-1, 0, 0}) == 2);
    assert(getResult(5, new int[5]{4, 0, 4, 0, -1}) == 3);
    assert(getResult(5, new int[5]{-1, 0, 0, 1, 2}) == 3);
    assert(getResult(6, new int[6]{4, 0, 4, 0, -1, 1}) == 3);
    assert(getResult(6, new int[6]{4, 5, 4, 0, -1, 0}) == 3);
}

void input() {
    int n;
    cin >> n;
    Graph g(n);
    int node;
    for (int i = 0; i < n; ++i) {
        cin >> node;
        g.addNode(i, node - 1);
    }

    cout << n - g.findMaxPairs();
}

int main() {
//    cout << getResult(3, "YYYYYYYYY");
//    test();
    input();
    return 0;
}
