#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cstdint>
#include <assert.h>
#include <string>
#include <cmath>
#include <queue>
#include <set>

using namespace std;
//
// Created by Anatolii on 2020-04-22.
//


class Edge {
public:
    Edge(int _from, int _to, int _capacity, bool original_, int number_) : from(_from), to(_to), capacity(_capacity),
                                                                           original(original_), flow(0),
                                                                           number(number_) {};

    Edge *reverse{};

    long long int remaining_capacity();

    void add_flow(long long int value);

    int from;
    int to;
    int number;

    long long int get_flow() {
        return flow;
    }

    long long int get_capacity() {
        return capacity;
    }

private:
    long long int flow;
    long long int capacity;
    // обычный и обратные ребра
    bool original;
};

long long int Edge::remaining_capacity() {
    return capacity - flow;
}

void Edge::add_flow(long long int value) {
    flow += value;
    reverse->flow -= value;
}

class Dinic {
public:
    Dinic(int n_) : n(n_), source(0), sink(n_ - 1), level(n), graph(n), start(n) {};

    void add_edge(int from, int to, int capacity, int number);

    bool bfs();

    long long int dfs(int at, long long int flow);

    long long int calc_flow();

    long long int calc_flow_one_launch();

    void calc_min_cut();

    long long int max_flow;
private:
    pair<struct Edge *, struct Edge *> add_one_direction_edge(int from, int to, int capacity, int number);

    long long int INF = LONG_LONG_MAX / 2;
    int n;
    int source;
    int sink;
    vector<int> start;
    vector<int> level;
    vector<vector<Edge *>> graph; // for each node - list of edges outgoing from that node
    vector<pair<Edge *, Edge *>> edges;
};

void Dinic::add_edge(int from, int to, int capacity, int number) {
    from--;
    to--;
    pair<Edge *, Edge *> forward = add_one_direction_edge(from, to, capacity, number);
    pair<Edge *, Edge *> backward = add_one_direction_edge(to, from, capacity, number);
    edges.emplace_back(forward.first, backward.first);
}

pair<Edge *, Edge *> Dinic::add_one_direction_edge(int from, int to, int capacity, int number) {
    Edge *forward = new Edge(from, to, capacity, true, number);
    Edge *reverse = new Edge(to, from, 0, false, number);
    forward->reverse = reverse;
    reverse->reverse = forward;
    graph.at(from).push_back(forward);
    graph.at(to).push_back(reverse);
    return make_pair(forward, reverse);

}

bool Dinic::bfs() {
    fill(level.begin(), level.end(), -1);
    fill(start.begin(), start.end(), 0);
    queue<int> q;
    q.push(source);
    level.at(source) = 0;
    int node;
    while (!q.empty()) {
        node = q.front();
        q.pop();
        int cur_level = level.at(node);
        for (Edge *edge: graph.at(node)) {
            if (edge->remaining_capacity() > 0 && level.at(edge->to) < 0) {
                level.at(edge->to) = cur_level + 1;
                q.push(edge->to);
            }
        }
    }
    return level.at(sink) != -1;
}

long long int Dinic::dfs(int at, long long int flow) {
    if (at == sink) return flow;

    for (int i = start.at(at); i < graph.at(at).size(); ++i) {
        Edge *edge = graph.at(at).at(i);
        long long int capacity = edge->remaining_capacity();
        if (capacity == 0) {
            start.at(at)++;
            continue;
        }

        if ((level.at(at) + 1) != level.at(edge->to)) continue;
        long long int max_additional_flow = dfs(edge->to, min(flow, edge->remaining_capacity()));
        if (max_additional_flow > 0) {
            graph.at(at).at(i)->add_flow(max_additional_flow);
            return max_additional_flow;
        }
    }
    return 0;
}

long long int Dinic::calc_flow_one_launch() {
    return dfs(source, INF);
}

long long int Dinic::calc_flow() {
    long long int flow = 0;
    long long int cur_flow = 0;
    while (bfs()) {
        cur_flow = calc_flow_one_launch();
        flow += cur_flow;
        while (cur_flow > 0) {
            cur_flow = calc_flow_one_launch();
            flow += cur_flow;
        }
    }
    max_flow = flow;
    return flow;
}

void Dinic::calc_min_cut() {
    vector<pair<int, long long int>> min_cut;
    vector<bool> visited(n);
    fill(visited.begin(), visited.end(), false);
    std::set<int> left;
    queue<int> q;
    q.push(source);
    int node;
    while (!q.empty()) {
        node = q.front();
        q.pop();
        if (visited.at(node)) continue;
        left.insert(node);
        visited.at(node) = true;
        for (Edge *edge: graph.at(node)) {
            if (edge->remaining_capacity() > 0 && !visited.at(edge->to)) {
                q.push(edge->to);
            }
        }
    }

    vector<int> cut_indexes;
    long long int min_cut_value = 0;
    for (int i = 0; i < edges.size(); ++i) {
        pair<Edge *, Edge *> edge = edges.at(i);
        int edge_start = edge.first->from;
        int edge_end = edge.first->to;
        if ((left.find(edge_start) != left.end() && left.find(edge_end) == left.end())
            || (left.find(edge_end) != left.end() && left.find(edge_start) == left.end())) {
            if (edge.first->remaining_capacity() == 0 || edge.second->remaining_capacity() == 0) {
                cut_indexes.push_back(i);
                min_cut_value += edge.first->get_capacity();
            }
        }
    }

    if (min_cut_value > max_flow) throw;
    int edge_count = cut_indexes.size();
    cout << edge_count << " " << min_cut_value << endl;
    for (auto &i : cut_indexes) {
        cout << i + 1 << " ";
    }
    cout << endl;
}

void input() {
    int n, m;
    cin >> n >> m;
    Dinic d(n);
    int from, to, cap;
    for (int i = 0; i < m; ++i) {
        cin >> from >> to >> cap;
        d.add_edge(from, to, cap, i);
    }
    d.calc_flow();
    d.calc_min_cut();
}

int main() {
    input();
    return 0;
}

