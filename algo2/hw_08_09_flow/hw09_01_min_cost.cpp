#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cstdint>
#include <assert.h>
#include <string>
#include <cmath>
#include <queue>


//A. Minimum Cost Maximum Flow
//time limit per test2 seconds
//        memory limit per test256 megabytes
//inputstandard input
//outputstandard output
//You are given a directed graph, each edge has a capacity and a cost of one unit of flow. Find the maximum flow with the minimum cost from vertex 1 to vertex 𝑛.
//
//Input
//        The first line contains 𝑛 and 𝑚 — the number of vertices and edges in the graph (2≤𝑛≤100, 0≤𝑚≤1000). Next 𝑚 lines contains four integers each: the beginning and the end vertex, the capacity and the cost. All costs and capacities do not exceed 105. It is guaranteed that the input graph does not contain negative cycles.
//
//Output
//        Output a single integer — the minimum cost of the maximum flow from 1 to 𝑛. The answer does not exceed 263−1. It is guaranteed that the input graph does not contain negative cycles.

//A. Максимальный поток минимальной стоимости
//ограничение по времени на тест2 секунды
//ограничение по памяти на тест256 мегабайт
//вводстандартный ввод
//выводстандартный вывод
//Задан ориентированный граф, каждое ребро которого обладает пропускной способностью и стоимостью. Найдите максимальный поток минимальной стоимости из вершины с номером 1 в вершину с номером 𝑛.
//
//Входные данные
//Первая строка входного файла содержит 𝑛 и 𝑚 — количество вершин и количество ребер графа (2≤𝑛≤100, 0≤𝑚≤1000). Следующие 𝑚 строк содержат по четыре целых числа числа: номера вершин, которые соединяет соответствующее ребро графа, его пропускную способность и его стоимость. Пропускные способности и стоимости не превосходят 105.
//
//Выходные данные
//В выходной файл выведите одно число — цену максимального потока минимальной стоимости из вершины с номером 1 в вершину с номером 𝑛. Ответ не превышает 263−1. Гарантируется, что в графе нет циклов отрицательной стоимости.
//
//Пример
//        входные данныеСкопировать
//4 5
//1 2 1 2
//1 3 2 2
//3 2 1 1
//2 4 2 1
//3 4 2 3
//выходные данныеСкопировать
//12

using namespace std;

class Edge {
public:
    Edge(int _from, int _to, int _capacity, bool original_, int cost_) : from(_from), to(_to), capacity(_capacity),
                                                                         original(original_), flow(0), cost(cost_) {};

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
    MinCost(int n_) : n(n_), source(0), sink(n_ - 1), graph(n), dist(n), prev(n), edge_to_parent(n) {};

    void add_edge(int from, int to, int capacity, int cost);

    void bellmand_ford();

    long long int one_step();

    long long int find_min_cost();

private:
    pair<struct Edge *, struct Edge *> add_one_direction_edge(int from, int to, int capacity, int cost);

    int n;
    int source;
    int sink;
    vector<long long int> dist;
    vector<int> prev;
    vector<Edge *> edge_to_parent;
    vector<vector<Edge *> > graph; // for each node - list of edges outgoing from that node
    vector<Edge *> edges;
};

void MinCost::add_edge(int from, int to, int capacity, int cost) {
    from--;
    to--;
    pair<Edge *, Edge *> forward = add_one_direction_edge(from, to, capacity, cost);
    edges.push_back(forward.first);
}

pair<Edge *, Edge *> MinCost::add_one_direction_edge(int from, int to, int capacity, int cost) {
    Edge *forward = new Edge(from, to, capacity, true, cost);
    Edge *reverse = new Edge(to, from, 0, false, -cost);
    forward->reverse = reverse;
    reverse->reverse = forward;
    graph.at(from).push_back(forward);
    graph.at(to).push_back(reverse);
    return make_pair(forward, reverse);

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
        for (Edge *edge: graph.at(cur)) {
            if (edge->remaining_capacity() == 0) continue;
            if (dist.at(edge->to) > dist.at(edge->from) + edge->get_cost()) {
                prev.at(edge->to) = edge->from;
                edge_to_parent.at(edge->to) = edge;
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
    bellmand_ford();
    if (dist.at(sink) == LONG_LONG_MAX) {
        return LONG_LONG_MIN;
    }
    int max_flow = INT_MAX;
    long long int cost = 0;
    int cur = sink;
    while (cur != source) {
        cost += edge_to_parent.at(cur)->get_cost();
        max_flow = min(max_flow, edge_to_parent.at(cur)->remaining_capacity());
        cur = prev.at(cur);
    }
    cur = sink;
    while (cur != source) {
        edge_to_parent.at(cur)->add_flow(max_flow);
        cur = prev.at(cur);
    }
    cost *= max_flow;
    return cost;
}

long long int MinCost::find_min_cost() {
    long long int min_cost = 0;
    long long int cur_cost = one_step();;
    while (cur_cost != LONG_LONG_MIN) {
        min_cost += cur_cost;
        cur_cost = one_step();
    }
    return min_cost;
}

void input() {
    int n, m;
    cin >> n >> m;
    MinCost d(n);
    int from, to, cap, cost;
    for (int i = 0; i < m; ++i) {
        cin >> from >> to >> cap >> cost;
        d.add_edge(from, to, cap, cost);
    }
    cout << d.find_min_cost();
}

int main() {
//    MinCost d(4);
//    d.add_edge(1, 2, 1, 2);
//    d.add_edge(1, 3, 2, 2);
//    d.add_edge(3, 2, 1, 1);
//    d.add_edge(2, 4, 2, 1);
//    d.add_edge(3, 4, 2, 3);
//    cout << d.find_min_cost();

    input();

    return 0;
}

