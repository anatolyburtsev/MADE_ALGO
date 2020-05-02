#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cstdint>
#include <assert.h>
#include <string>
#include <cmath>
#include <queue>

using namespace std;

class Edge {
public:
    Edge(int _from, int _to, int _capacity, bool original_) : from(_from), to(_to), capacity(_capacity),
                                                              original(original_), flow(0) {};

    Edge *reverse{};

    int remaining_capacity();

    void add_flow(int value);

    int from;
    int to;

    int get_flow() {
        return flow;
    }

private:
    int flow;
    int capacity;
    // обычный и обратные ребра
    bool original;
    // изначальные ребра могут быть развернуты backward = true
    bool backward;
};

int Edge::remaining_capacity() {
    return capacity - flow;
}

void Edge::add_flow(int value) {
    flow += value;
    reverse->flow -= value;
}

class Dinic {
public:
    Dinic(int n_) : n(n_), source(0), sink(n_ - 1), level(n), graph(n), start(n) {};

    void add_edge(int from, int to, int capacity);

    bool bfs();

    int dfs(int at, int flow);

    long long int calc_flow();

    int calc_flow_one_launch();

    void answer();

    long long int max_flow;
private:
    pair<struct Edge *, struct Edge *> add_one_direction_edge(int from, int to, int capacity);

    int INF = INT_MAX / 2;
    int n;
    int source;
    int sink;
    vector<int> start;
    vector<int> level;
    vector<vector<Edge *>> graph; // for each node - list of edges outgoing from that node
    vector<pair<Edge *, Edge *>> edges;
};

void Dinic::add_edge(int from, int to, int capacity) {
    from--;
    to--;
    pair<Edge *, Edge *> forward = add_one_direction_edge(from, to, capacity);
    pair<Edge *, Edge *> backward = add_one_direction_edge(to, from, capacity);
    edges.emplace_back(forward.first, backward.first);
}

pair<Edge *, Edge *> Dinic::add_one_direction_edge(int from, int to, int capacity) {
    Edge *forward = new Edge(from, to, capacity, true);
    Edge *reverse = new Edge(to, from, 0, false);
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

int Dinic::dfs(int at, int flow) {
    if (at == sink) return flow;

    for (int i = start.at(at); i < graph.at(at).size(); ++i) {
        Edge *edge = graph.at(at).at(i);
        int capacity = edge->remaining_capacity();
        if (capacity == 0) {
            start.at(at)++;
            continue;
        }

        if ((level.at(at) + 1) != level.at(edge->to)) continue;
        int max_additional_flow = dfs(edge->to, min(flow, edge->remaining_capacity()));
        if (max_additional_flow > 0) {
            graph.at(at).at(i)->add_flow(max_additional_flow);
            return max_additional_flow;
        }
    }
    return 0;
}

int Dinic::calc_flow_one_launch() {
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

void Dinic::answer() {
    cout << max_flow << endl;
    pair<Edge *, Edge *> edge;
    for (int i = 0; i < edges.size(); ++i) {
        edge = edges.at(i);
        if (edge.first->get_flow() > 0) {
            cout << edge.first->get_flow() << endl;
        } else {
            if (edge.second->get_flow() > 0) {
                cout << "-" << edge.second->get_flow() << endl;
            } else {
                cout << 0 << endl;
            }
        }
    }

}

//
// Created by Anatolii on 2020-04-20.
//

void input() {
    int n, m;
    cin >> n >> m;
    Dinic d(n);
    int from, to, cap;
    for (int i = 0; i < m; ++i) {
        cin >> from >> to >> cap;
        d.add_edge(from, to, cap);
    }
    d.calc_flow();
    d.answer();
}

int main() {
//    Dinic d(5);
//    d.add_edge(1, 2, 1);
//    d.add_edge(1, 3, 3);
//    d.add_edge(1, 3, 3);
//    d.add_edge(3, 4, 6);
//    d.add_edge(4, 2, 2);
//    d.add_edge(4, 5, 10);

//    Dinic d(4);
//    d.add_edge(0, 1, 2);
//    d.add_edge(1, 3, 1);
//    d.add_edge(0, 2, 1);
//    d.add_edge(2, 3, 2);

//    Dinic d(3);
//    d.add_edge(1, 0, 2);
//    d.add_edge(1, 2, 1);

//    d.bfs();
//    cout << d.calc_flow_one_launch() << endl;
//    cout << d.calc_flow_one_launch() << endl;
//    cout << d.calc_flow_one_launch() << endl;
//    cout << d.calc_flow_one_launch() << endl;
//    Dinic d(2);
//    d.add_edge(1, 2, 1);
//    d.add_edge(2, 1, 3);
//
//    d.calc_flow();
//    d.answer();
    input();
    return 0;
}

