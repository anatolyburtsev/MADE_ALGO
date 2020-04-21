#include <iostream>
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

vector<int> precalc(const vector<int> &p, int max_depth) {
    int n = p.size();
    int k_max = ceil(log(max_depth)) + 1;
    vector<int> jmp(n * k_max);
    for (int v = 0; v < n; ++v) {
        jmp.at(v * k_max) = p.at(v);
    }
    for (int k = 1; k < k_max; ++k) {
        for (int v = 0; v < n; ++v) {
            jmp.at(v * k_max + k) = jmp.at(jmp.at(v * k_max + k - 1) * k_max + k - 1);
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


int lca(int u, int v, const vector<int> &p, const vector<int> &d, int max_depth, const vector<int> &jmp) {
    if (d.at(u) < d.at(v)) {
        swap(u, v);
    }
    int delta = d.at(u) - d.at(v);
    int k_max = ceil(log(max_depth)) + 1;

    // bamboo tree
    if (delta > 0) {
        for (int k = k_max - 1; k >= 0; --k) {
            if (delta >= (1 << k)) {
                u = jmp_at(u, k, jmp, k_max);
                delta -= (1 << k);
            }
        }
    }
//    if (d.at(u) != d.at(v)) throw;
    if (u == v) {
        return u;
    }
//    return u;
    for (int k = k_max; k >= 0; k--) {
        int u1 = jmp_at(u, k, jmp, k_max);
        int v1 = jmp_at(v, k, jmp, k_max);
        if (u1 != v1) {
            u = u1;
            v = v1;
        }
    }
    return jmp.at(u * k_max); // try p(u)
}

int lca_half(int u, int v, const vector<int> &d, const vector<int> &jmp, const vector<int> &p) {
    if (d.at(u) < d.at(v)) {
        swap(u, v);
    }
    int delta = d.at(u) - d.at(v);
    int k_max = ceil(log(d.size())) + 1;

    for (int i = 0; i < delta; ++i) {
        u = p.at(u);
    }
//    for (int k = k_max; k >= 0; k--) {
//        if (delta >= (1 << k)) {
//            u = jmp_at(u, k, jmp, k_max);
//            delta -= (1 << k);
//        }
//    }
    if (u == v) {
        return u;
    }
    return u;

    for (int k = k_max; k >= 0; k--) {
        int u1 = jmp_at(u, k, jmp, k_max);
        int v1 = jmp_at(v, k, jmp, k_max);
        if (u1 != v1) {
            u = u1;
            v = v1;
        }
    }
    return jmp.at(u * k_max);
}

int lca_naive(int u, int v, const vector<int> &d, const vector<int> &p) {
    if (d.at(u) < d.at(v)) {
        swap(u, v);
    }
    int delta = d.at(u) - d.at(v);
    for (int i = 0; i < delta; ++i) {
        u = p.at(u);
    }
    while (u != v) {
        u = p.at(u);
        v = p.at(v);
    }
    return u;
}

//void test() {
//    vector<int> p{0, 0, 0, 2, 2};
//    vector<int> d = calc_depth(p);
//    vector<int> jmp = precalc(p);
//    assert(lca(1, 1, d, jmp) == 1);
//    assert(lca(1, 2, d, jmp) == 0);
//    assert(lca(1, 3, d, jmp) == 0);
//    assert(lca(4, 3, d, jmp) == 2);
//    assert(lca(1, 4, d, jmp) == 0);
//    assert(lca(2, 3, d, jmp) == 2);
//    assert(lca(0, 4, d, jmp) == 0);
//}

void input() {
    int n, m, u, v;
    cin >> n;
    vector<int> p(n);
    p.at(0) = 0;
    int tmp;
    for (int i = 1; i < n; i++) {
        cin >> tmp;
        p.at(i) = tmp - 1;
    }
    vector<int> d = calc_depth(p);
    int max_depth = INT_MIN;
    for (auto di: d) {
        if (di > max_depth) max_depth = di;
    }
    vector<int> jmp = precalc(p, max_depth);
    cin >> m;
    for (int i = 0; i < m; ++i) {
        cin >> u >> v;
//        cout << lca_half(u - 1, v - 1, d, jmp, p) + 1 << endl;
        cout << lca(u - 1, v - 1, p, d, max_depth, jmp) + 1 << endl;
    }
}
//
//void t() {
//    int n = 1000;
//    int m = 500;
//    vector<int> p(n);
//    p.at(0) = 0;
//    for (int i = 1; i < n; ++i) {
//        p.at(i) = rand() % i;
//    }
//    vector<int> d = calc_depth(p);
//    vector<int> jmp = precalc(p);
//
//    for (int i = 0; i < m; ++i) {
//        int u = rand() % n;
//        int v = rand() % n;
//
//        int a = lca(u, v, d, jmp);
////        int b = lca_half(u, v, d, jmp, p);
//        int c = lca_naive(u, v, d, p);
//        if (a != c) {
//            cout << "Found" << endl;
//        }
//    }
//}

int main() {
//    for (int i = 0; i < 10000000; ++i) {
//        if (i % 10000 == 0) {
//            cout << i << endl;
//        }
//        t();
//    }
    input();
//    test();
    return 0;
}
