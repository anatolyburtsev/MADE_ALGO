#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdint>
#include <assert.h>
#include <string>

using namespace std;

//
// Created by Anatolii on 2020-03-07.
//
class Graph {
public:
    Graph(int n) {
        this->n = n;
        this->nodes = new vector<int>(n * n);
        this->dp = new vector<vector<int>>(n);
        this->p = new vector<vector<int>>(n);
        for (int i = 0; i < n; ++i) {
            this->dp->at(i).resize(1 << n);
            this->p->at(i).resize(1 << n);
        }
    }

    ~Graph() {
        this->nodes->clear();
    }

    void addNode(int index, int dist) {
        this->nodes->at(index) = dist;
    }

    int getDist(int s, int d);

    int getRoute();

    static int getBit(int mask, int i) {
        return (mask >> i) & 1;
    }

private:
    int n;
    vector<int> *nodes;
    vector<vector<int>> *dp;
    vector<vector<int>> *p;

};


int Graph::getRoute() {
    // dp[v][mask] - min route to get to v through mask
    for (int i = 0; i < n; ++i) {
        fill(dp->at(i).begin(), dp->at(i).end(), INT_MAX);
        dp->at(i).at(0) = 0;
    }
    for (int mask = 1; mask < 1 << n; ++mask) {
        for (int v = 0; v < n; ++v) {
            if (getBit(mask, v) == 1) continue;
            int min_route = INT_MAX;
            int best_parent = -1;
            for (int u = 0; u < n; ++u) {
                if (getBit(mask, u) != 1) continue;
                if (u == v) continue;
                int cur_route = dp->at(u).at(mask - (1 << u)) + getDist(u, v);
                if (cur_route < min_route) {
                    min_route = cur_route;
                    best_parent = u;
                }
            }
            dp->at(v).at(mask) = min_route;
            p->at(v).at(mask) = best_parent;
        }
    }
    int min_route = INT_MAX;
    int start = -1;
    for (int j = 0; j < n; ++j) {
        int cur_route = dp->at(j).at((1 << n) - 1 - (1 << j));
        if (cur_route < min_route) {
            min_route = cur_route;
            start = j;
        }
    }

    cout << min_route << endl;

    cout << start +1 << " ";
    int mask = (1<< n) - 1 - (1<<start);
    int cur_pos = start;
    while (mask > 0) {
        int next_parent = p->at(cur_pos).at(mask);
        cur_pos = next_parent;
        cout << next_parent+1 << " ";
        mask -= 1<< next_parent;
    }
    return min_route;
}

int Graph::getDist(int s, int d) {
    return nodes->at(s * n + d);
}

void input() {
    int n;
    cin >> n;
    Graph g(n);
    int tmp;
    for (int i = 0; i < n * n; ++i) {
        cin >> tmp;
        g.addNode(i, tmp);
    }
    g.getRoute();
}

int test_set(int n, int *nodes) {
//    int n = 4;
    Graph g(n);
//    int *nodes = new int[n * n]{0, 1, 10, 1, 1, 0, 10, 1, 1, 10, 0, 1, 10, 1, 1, 0};
    for (int i = 0; i < n * n; ++i) {
        g.addNode(i, nodes[i]);
    }
    return g.getRoute();
}

void test() {
//    cout << test_set(5, new int[5*5]{0,8,1,6,7,8,0,2,5,4,1,2,0,9,10,6,5,9,0,3,7,4,10,3,0}) ;
    assert(test_set(5, new int[5*5]{0, 183, 163, 173, 181, 183, 0, 165, 172, 171, 163, 165, 0, 189, 302, 173, 172, 189, 0, 167, 181, 171, 302, 167, 0}) == 666);
//    assert(test_set(3, new int[9]{0, 10, 4, 10, 0, 2, 4, 2, 0}) == 6);
//    assert(test_set(4, new int[16]{0, 1, 1, 1, 1, 0, 10, 1, 1, 10, 0, 1, 10, 1, 1, 0}) == 3);
//    assert(test_set(4, new int[16]{0,1,10,1,1,0,10,10,10,10,0,1,1,10,10,3}) == 3);
}

int main() {
//    test();
    input();

    return 0;
}
