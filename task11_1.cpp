//
// Created by Анатолий Бурцев on 2019-12-02.
// Дан невзвешенный неориентированный граф. Найдите цикл минимальной длины.
//
#include <vector>
#include <iostream>
#include <queue>

class Graph {
public:
    void addEdge(int src, int dst);

    Graph(int n) {
        this->n = n;
        for (int i = 0; i < n; ++i) {
            std::vector<int> *v = new std::vector<int>();
            for (int j = 0; j < n; ++j) {
                v->push_back(NO_EDGE_CONST);
            }
            edges.push_back(v);
        }
    }

    struct Node {
        int node;
        int src_node;

        explicit Node(int _node, int _src_node) : node(_node), src_node(_src_node) {}
    };

    int find_loop_from_position(int start_position);

    int find_min_loop();

    std::vector<int> *getSiblingsExceptSrc(int node, int src);

private:
    int n;
    std::vector<std::vector<int> *> edges;
    int NO_EDGE_CONST = -1;
};

void Graph::addEdge(int src, int dst) {
    edges.at(src)->at(dst) = 1;
    edges.at(dst)->at(src) = 1;
}

int Graph::find_loop_from_position(int start_position) {
    int NOT_VISITED_NODE = -1;

    std::vector<int> states;
    states.reserve(n);
    for (int i = 0; i < n; ++i) {
        states.push_back(NOT_VISITED_NODE);
    }
    std::queue<Node *> queue;
    std::vector<int> *siblings;
    states.at(start_position) = 0;
    queue.push(new Node(start_position, -1));

    // BFS
    while (!queue.empty()) {
        Node *node = queue.front();
        queue.pop();
        siblings = getSiblingsExceptSrc(node->node, node->src_node);
        int layer = states.at(node->node);

        for (int next_node: *siblings) {
            if (states.at(next_node) == NOT_VISITED_NODE) {
                states.at(next_node) = layer + 1;
                queue.push(new Node(next_node, node->node));
            } else {
                delete siblings;
                delete node;
                return layer + states.at(next_node) + 1;
            }
        }

        delete siblings;
        delete node;
    }
    return -1;
}

std::vector<int> *Graph::getSiblingsExceptSrc(int node, int src) {
    auto *sibl = new std::vector<int>();
    std::vector<int> *edges_from_this_node = edges.at(node);
    for (int i = 0; i < n; ++i) {
        if (edges_from_this_node->at(i) > 0) {
            if (src < 0 || i != src) {
                sibl->push_back(i);
            }
        }
    }
    return sibl;
}

int Graph::find_min_loop() {
    int INITIAL_BIG_VALUE = 1000000;
    int min_loop = INITIAL_BIG_VALUE;
    for (int i = 0; i < n; ++i) {
        int loop_size = find_loop_from_position(i);
        if (loop_size > 0 && loop_size < min_loop) {
            min_loop = loop_size;
        }
    }
    if (min_loop == INITIAL_BIG_VALUE) {
        return -1;
    } else {
        return min_loop;
    }
}

int main() {
    int n, k;
    int src, dst;
    std::cin >> n >> k;
    Graph g(n);
    for (int i = 0; i < k; ++i) {
        std::cin >> src >> dst;
        g.addEdge(src, dst);
    }
    std::cout << g.find_min_loop() << std::endl;
    return 0;
}
