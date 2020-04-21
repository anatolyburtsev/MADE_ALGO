#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cstdint>
#include <assert.h>
#include <string>
#include <cmath>

using namespace std;

//
// Created by Anatolii on 2020-04-10.
//

void swap(int &a, int &b) {
    int tmp = a;
    a = b;
    b = tmp;
}

void find_depth(int node, const vector<int> &p, vector<int> &d) {
    if (d.at(node) >= 0) return;
    int parent = p.at(node);
    int parent_depth = d.at(parent);
    if (parent_depth < 0) {
        find_depth(parent, p, d);
        parent_depth = d.at(parent);
    }
    d.at(node) = parent_depth + 1;
}

vector<int> calc_depth(const vector<int> &p) {
    vector<int> d(p.size());
    fill(d.begin(), d.end(), -1);
    d.at(0) = 0;
    for (int i = 1; i < p.size(); ++i) {
        find_depth(i, p, d);
    }
    return d;
}

vector<int> precalc(const vector<int> &p, const vector<int> &edges, int max_depht, vector<int> &mins) {
    int n = p.size();
    int k_max = ceil(log(max_depht)) + 1;
    vector<int> jmp(n * k_max);
    for (int v = 0; v < n; ++v) {
        jmp.at(v * k_max) = p.at(v);
        mins.at(v * k_max) = edges.at(v);
    }
    for (int k = 1; k < k_max; ++k) {
        for (int v = 0; v < n; ++v) {
            jmp.at(v * k_max + k) = jmp.at(jmp.at(v * k_max + k - 1) * k_max + k - 1);
            mins.at(v * k_max + k) = min(mins.at(v * k_max + k - 1),
                                         mins.at(jmp.at(v * k_max + k - 1) * k_max + k - 1));
        }
    }
    return jmp;
}

int jmp_at(int x, int y, const vector<int> &jmp, int line_size) {
    if (y < line_size) {
        return jmp.at(x * line_size + y);
    } else {
        return jmp.at(x * line_size + line_size - 1);
    }

}

int min_at(int x, int y, const vector<int> &mins, int line_size) {
    if (y < line_size) {
        return mins.at(x * line_size + y);
    } else {
        return mins.at(x * line_size + line_size - 1);
    }

}


int lca(int u, int v, const vector<int> &d, const vector<int> &jmp, const vector<int> &mins, int max_depth) {
    if (d.at(u) < d.at(v)) {
        swap(u, v);
    }
    int delta = d.at(u) - d.at(v);
    int k_max = ceil(log(max_depth)) + 1;

    int min_answer = INT_MAX;

    for (int k = k_max - 1; k >= 0; k--) {
        if (delta >= (1 << k)) {
            min_answer = min(min_answer, mins.at(u * k_max + k));
            u = jmp.at(u * k_max + k);
            delta -= (1 << k);
        }
        if (delta == 0) break;
    }
    if (d.at(u) != d.at(v)) throw;
    if (u == v) {
        return min_answer;
    }
    for (int k = k_max; k >= 0; k--) {
        int u1 = jmp_at(u, k, jmp, k_max);
        min_answer = min(min_answer, min_at(u, k, mins, k_max));
        int v1 = jmp_at(v, k, jmp, k_max);
        min_answer = min(min_answer, min_at(v, k, mins, k_max));
        if (u1 != v1) {
            u = u1;
            v = v1;
        }
    }
    return min_answer;
}

void input() {
    ifstream inFile;
    inFile.open("minonpath.in");
    int n, m, u, v;
    inFile >> n;
    vector<int> p(n);
    vector<int> edges(n);
    edges.at(0) = INT_MAX;
    p.at(0) = 0;
    int tmp;
    int edg_tmp;
    for (int i = 1; i < n; i++) {
        inFile >> tmp >> edg_tmp;
        p.at(i) = tmp - 1;
        edges.at(i) = edg_tmp;
    }
    vector<int> d = calc_depth(p);
    int max_depth = INT_MIN;
    for (auto di: d) {
        if (di > max_depth) max_depth = di;
    }
    vector<int> mins(n * max_depth);

    vector<int> jmp = precalc(p, edges, max_depth, mins);
    inFile >> m;
    ofstream outFile;
    outFile.open("minonpath.out");
    for (int i = 0; i < m; ++i) {
        inFile >> u >> v;
        outFile << lca(u - 1, v - 1, d, jmp, mins, max_depth) << endl;
    }
    outFile.close();
}

int main() {
    input();
//    test();
    return 0;
}
