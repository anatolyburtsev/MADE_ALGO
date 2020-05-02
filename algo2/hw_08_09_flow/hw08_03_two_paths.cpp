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
    Dinic(int n_, int m_, int source_, int sink_) : n(n_), m(m_), source(source_), sink(sink_), level(n),
                                                    graph(n), start(n) {};

    void add_edge(int from, int to, int capacity, int number);

    bool bfs();

    long long int dfs(int at, long long int flow);

    long long int calc_flow();

    long long int calc_flow_one_launch();

    long long int max_flow;

    void find_path();

private:
    pair<struct Edge *, struct Edge *> add_one_direction_edge(int from, int to, int capacity, int number);

    long long int INF = LONG_LONG_MAX / 2;
    int n; // no of nodes
    int m; // no of edges
    int source;
    int sink;
    vector<int> start;
    vector<int> level;
    vector<vector<Edge *>> graph; // for each node - list of edges outgoing from that node
    vector<Edge *> edges;
};

void Dinic::add_edge(int from, int to, int capacity, int number) {
    from--;
    to--;
    pair<Edge *, Edge *> forward = add_one_direction_edge(from, to, capacity, number);
    edges.push_back(forward.first);
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
//    int i = 0;
    while (bfs()) {
//        cout << "run: " << i << endl;
        cur_flow = calc_flow_one_launch();
//        cout << "initial cur flow: " << cur_flow << endl;
        flow += cur_flow;
        while (cur_flow > 0) {
            cur_flow = calc_flow_one_launch();
//            cout << "cur flow: " << cur_flow << endl;
            flow += cur_flow;
        }
    }
    max_flow = flow;
    return flow;
}

void Dinic::find_path() {
    if (max_flow < 2) {
        cout << "NO" << endl;
        return;
    }

    cout << "YES" << endl;
    vector<bool> used_edges(m);
    fill(used_edges.begin(), used_edges.end(), false);
    int current_node = source;
    while (current_node != sink) {
        for (Edge *edge: graph.at(current_node)) {
            if (edge->get_flow() > 0) {
//                cout << "here for node: " << current_node << endl;
                used_edges.at(edge->number) = true;
                cout << edge->from + 1 << " ";
                current_node = edge->to;
                break;
            }
        }
    }
    cout << sink + 1 << endl;

    current_node = source;
    while (current_node != sink) {
        for (Edge *edge: graph.at(current_node)) {
            if (edge->get_flow() > 0 && !used_edges.at(edge->number)) {
//                cout << "here for node: " << current_node << endl;
                used_edges.at(edge->number) = true;
                cout << edge->from + 1 << " ";
                current_node = edge->to;
                break;
            }
        }
    }
    cout << sink + 1 << endl;

}

void input() {
    int n, m, s, t;
    cin >> n >> m >> s >> t;
    Dinic d(n, m, s - 1, t - 1);
    int from, to, cap;
    for (int i = 0; i < m; ++i) {
        cin >> from >> to;
        if (from == to) continue;
        d.add_edge(from, to, 1, i);
    }
    d.calc_flow();
    d.find_path();
}

int main() {
    input();
    return 0;
}

