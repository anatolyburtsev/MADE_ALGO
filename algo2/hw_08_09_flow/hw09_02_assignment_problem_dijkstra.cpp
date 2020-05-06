#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cstdint>
#include <assert.h>
#include <string>
#include <cmath>
#include <ctime>
#include <queue>

//B. Assignment Problem
//time limit per test5.0 s
//        memory limit per test256 megabytes
//inputstandard input
//outputstandard output
//You are given an integer matrix 𝐶 of size 𝑛×𝑛. You should choose 𝑛 cells in such a way that each row and each column contain exactly one chosen cell, and the sum of values of all chosen cells is as small as possible.
//
//Input
//        The first line contains 𝑛 (2≤𝑛≤300). Next 𝑛 lines contains 𝑛 integers 𝐶𝑖𝑗 All values are non-negative and do not exceed 106.
//
//Output
//        The first line should contains the minimum possible sum. Next 𝑛 lines should contains two integers each — the row and the column of the chosen cell. You can print cells in any order.
//
//Example
//        inputCopy
//3
//3 2 1
//1 3 2
//2 1 3
//outputCopy
//3
//2 1
//3 2
//1 3

using namespace std;

class Edge {
public:
    Edge(int _from, int _to, int _capacity, bool original_, int cost_) : from(_from), to(_to), capacity(_capacity),
                                                                         original(original_), flow(0),
                                                                         cost(cost_) {};

    Edge *reverse{};

    int remaining_capacity();

    void add_flow(int value);

    int from;
    int to;

    int get_flow() {
        return flow;
    }

    int get_cost() {
        return cost;
    }

private:
    int flow;
    int capacity;
    int cost;
    // обычный и обратные ребра
    bool original;
};

int Edge::remaining_capacity() {
    return capacity - flow;
}

void Edge::add_flow(int value) {
    flow += value;
    reverse->flow -= value;
}

class MinCost {
public:
    MinCost(int n_, int orig_n_) : n(n_), orig_n(orig_n_), source(0), sink(n_ - 1), dist(n), prev(n),
                                   phi(n), edges(n * n) {};

    void add_edge(int from, int to, int capacity, int cost);

    void dijkstra_valid();

    long long int one_step();

    long long int find_min_cost();

    void answer();

    void bellmand_ford();

    long long int get_edge_weight(Edge *edge);

    Edge *get_edge(int from, int to);

    void add_edge(int from, int to, Edge *edge);

private:
    void add_one_direction_edge(int from, int to, int capacity, int cost);

    int n;
    int orig_n;
    int source;
    int sink;
    vector<long long int> phi;
    vector<long long int> dist;
    vector<int> prev;
    vector<Edge *> edges;
};

void MinCost::add_edge(int from, int to, int capacity, int cost) {
    add_one_direction_edge(from, to, capacity, cost);
}

void MinCost::add_one_direction_edge(int from, int to, int capacity, int cost) {
    Edge *forward = new Edge(from, to, capacity, true, cost);
    Edge *reverse = new Edge(to, from, 0, false, -cost);
    forward->reverse = reverse;
    reverse->reverse = forward;
    add_edge(from, to, forward);
    add_edge(to, from, reverse);
}


void MinCost::dijkstra_valid() {
    vector<bool> visited(n);
    fill(visited.begin(), visited.end(), false);
    long long int INF = LONG_LONG_MAX / 10;
    fill(dist.begin(), dist.end(), INF);
    dist.at(source) = 0;

    int index = 0;
    int u = 0;
    for (int i = 0; i < n; ++i) {
        long long int min = LONG_LONG_MAX;
        for (int j = 0; j < n; ++j) {
            if (!visited.at(j) && dist.at(j) < min) {
                min = dist.at(j);
                index = j;
            }
        }
        u = index;
        visited.at(u) = true;
        for (int j = 0; j < n; ++j) {
            Edge *edge = get_edge(u, j);
            if (edge == nullptr || edge->remaining_capacity() == 0) continue;
            long long int edge_weight = get_edge_weight(edge);
            if (!visited.at(j) && dist.at(u) != INF && (dist.at(u) + edge_weight < dist.at(j))) {
                dist.at(j) = dist.at(u) + edge_weight;
                prev.at(j) = u;
            }
        }
    }
}

void MinCost::bellmand_ford() {
    int cur;
    vector<bool> node_in_queue(n);
    fill(node_in_queue.begin(), node_in_queue.end(), false);
    fill(dist.begin(), dist.end(), LONG_LONG_MAX);
    dist.at(0) = 0;
    node_in_queue.at(0) = true;
    queue<int> q;
    q.push(source);
    while (!q.empty()) {
        cur = q.front();
        q.pop();
        for (int to = 0; to < n; ++to) {
            Edge *edge = get_edge(cur, to);
            if (edge == nullptr) continue;
            if (edge->remaining_capacity() == 0) continue;
            if (dist.at(edge->to) > dist.at(edge->from) + edge->get_cost()) {
                prev.at(edge->to) = edge->from;
                dist.at(edge->to) = dist.at(edge->from) + edge->get_cost();
                if (!node_in_queue.at(edge->to)) {
                    q.push(edge->to);
                    node_in_queue.at(edge->to) = true;
                }
            }
        }
        node_in_queue.at(cur) = false;
    }
}

long long int MinCost::one_step() {
    for (int i = 0; i < n; ++i) {
        phi.at(i) += dist.at(i);
    }
    dijkstra_valid();
    if (dist.at(sink) == LONG_LONG_MAX) {
        return -1;
    }
    int max_flow = INT_MAX;
    long long int cost = 0;
    int cur = sink;
    while (cur != source) {
        Edge *edge = get_edge(prev.at(cur), cur);
        cost += edge->get_cost();
        max_flow = min(max_flow, edge->remaining_capacity());
        cur = prev.at(cur);
    }
    cur = sink;
    while (cur != source) {
        get_edge(prev.at(cur), cur)->add_flow(max_flow);
        cur = prev.at(cur);
    }
    return cost * max_flow;
}

long long int MinCost::find_min_cost() {
    bellmand_ford();
    fill(phi.begin(), phi.end(), 0);

    long long int cur_cost = one_step();
    long long int min_cost = cur_cost;
    for (int i = 1; i < orig_n; ++i) {
        cur_cost = one_step();
        min_cost += cur_cost;
    }
    return min_cost;
}

void MinCost::answer() {
    long long int cost = 0;
    vector<int> best_edges(orig_n + 1);
    for (int node = 1; node < orig_n + 1; ++node) {
        for (int to = 0; to < n; ++to) {
            Edge *edge = get_edge(node, to);
            if (edge == nullptr) continue;
            if (edge->get_flow() > 0) {
                cost += edge->get_cost();
                best_edges.at(node) = edge->to - orig_n;
                continue;
            }
        }
    }
    cout << cost << endl;
    for (int i = 1; i < orig_n + 1; ++i) {
        cout << i << " " << best_edges.at(i) << endl;
    }
}

long long int MinCost::get_edge_weight(Edge *edge) {
    long long int weight = edge->get_cost() + phi.at(edge->from) - phi.at(edge->to);
    if (weight < 0) {
        cout << weight << endl;
    }
    return weight;
}

void MinCost::add_edge(int from, int to, Edge *edge) {
    edges.at(from * n + to) = edge;
}

Edge *MinCost::get_edge(int from, int to) {
    return edges.at(from * n + to);
}

void input_d() {
    std::ios::sync_with_stdio(false), std::cin.tie(0), std::cout.tie(0);
    int n;
    cin >> n;
    MinCost d(2 * n + 2, n);
    for (int j = 0; j < n; ++j) {
        d.add_edge(0, j + 1, 1, 0);
        d.add_edge(n + 1 + j, 2 * n + 1, 1, 0);
    }
    int v;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cin >> v;
            d.add_edge(i + 1, n + 1 + j, 1, v);
        }
    }
    d.find_min_cost();
    d.answer();
}

int main() {
    input_d();

    return 0;
}

