#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdint>
#include <assert.h>
#include <string>
#include <cmath>

using namespace std;

//
// Created by Anatolii Burtsev on 2020-03-29.
//

int NEUTRAL_ELEMENT = INT_MAX;

//void add_elements_to_power_of_two(vector<int> &a) {
//    int m = 1;
//    while (m < a.size()) {
//        m *= 2;
//    }
//    for (int i = a.size(); i < m; ++i) {
//        a.push_back(NEUTRAL_ELEMENT);
//    }
//}

class SegmentTree {
public:
    SegmentTree(vector<long long int> &a);

    long long int min_in_interval(int a, int b);

    void add_to_group(int a, int b, long long int k);

    void set_in_group(int a, int b, long long int k);

private:
    void add(int v, int l, int r, int a, int b, long long int k);

    void set(int i, long long int k);

    long long int rmq(int v, int l, int r, int a, int b);

    void push(int v, int l, int r);

    long long int get(int v, int l, int r);

    vector<long long int> t;
    vector<long long int> upd;
    int x;
    int real_size;
    int n;

};

SegmentTree::SegmentTree(vector<long long int> &a) {
    int x = 1;
    int n = a.size();
    this->n = n;
    while (x < n) {
        x *= 2;
    }

    for (int i = n; i < x; ++i) {
        a.push_back(NEUTRAL_ELEMENT);
    }
    this->x = x;
    this->real_size = x;
    vector<long long int> t(2 * x - 1);
    vector<long long int> upd(2 * x - 1);
    fill(upd.begin(), upd.end(), 0);
    this->upd = upd;

    for (int i = 0; i < x; ++i) {
        t.at(i + x - 1) = a.at(i);
    }

    for (int v = x - 2; v >= 0; --v) {
        t.at(v) = min(t.at(2 * v + 1), t.at(2 * v + 2));
    }
    this->t = t;
}

void SegmentTree::set(int i, long long int k) {
    int v = i + x - 1;
    t.at(i + x - 1) = k;
    while (v != 0) {
        v = (v - 1) / 2;
        t.at(v) = min(t.at(2 * v + 1), t.at(2 * v + 2));
    }
}

long long int SegmentTree::rmq(int v, int l, int r, int a, int b) {
    if (v >= t.size()) return NEUTRAL_ELEMENT;
    push(v, l, r);
    if (l > b || r < a) {
        return NEUTRAL_ELEMENT;
    }
    if (l >= a && r <= b) {
        return get(v, l, r);
    }
    int m = (l + r) / 2;

    return min(rmq(2 * v + 1, l, m, a, b), rmq(2 * v + 2, m + 1, r, a, b));
}

long long int SegmentTree::get(int v, int l, int r) {
    return t.at(v) + upd.at(v);
}

void SegmentTree::push(int v, int l, int r) {
    if (l == r) {
        t.at(v) += upd.at(v);
    } else {
        upd.at(2 * v + 1) += upd.at(v);
        upd.at(2 * v + 2) += upd.at(v);
        int m = (l + r) / 2;
        t.at(v) = min(get(2 * v + 1, l, m), get(2 * v + 2, m + 1, r));
    }
    upd.at(v) = 0;
}

void SegmentTree::add(int v, int l, int r, int a, int b, long long int k) {
    push(v, l, r);
    if (l > b || r < a) return;
    if (l >= a && r <= b) {
        upd.at(v) += k;
        return;
    }
    int m = (l + r) / 2;
    add(2 * v + 1, l, m, a, b, k);
    add(2 * v + 2, m + 1, r, a, b, k);
    t.at(v) = min(get(2 * v + 1, l, m), get(2 * v + 2, m + 1, r));
}

long long int SegmentTree::min_in_interval(int a, int b) {
    return rmq(0, 0, real_size - 1, a, b);
}

void SegmentTree::add_to_group(int a, int b, long long int k) {
    add(0, 0, real_size - 1, a, b, k);
}

void SegmentTree::set_in_group(int a, int b, long long int k) {
    push(0, a, b);
    for (int i = a; i <= b; ++i) {
        set(i, k);
    }
}

//void SegmentTree::set(int v, int l, int r, int a, int b, long long int x) {
//    push(v, l, r);
//    if (l > b || r < a) return;
//    if (l >= a && r <= b) {
//        upd.at(v) = x;
//        return;
//    }
//    int m = (l + r) / 2;
//    add(2 * v + 1, l, m, a, b, x);
//    add(2 * v + 2, m + 1, r, a, b, x);
//    t.at(v) = min(get(2 * v + 1, l, m), get(2 * v + 2, m + 1, r));
//}

void t1() {
    vector<long long int> a{1, 2, 3, 4};//, 4, 5, 6, 7};
    SegmentTree st(a);
    assert (st.min_in_interval(0, 1) == 1);
}

void t2() {
    vector<long long int> a{1, 2, 3, 4, 5, 6, 7};
    SegmentTree st(a);
    st.add_to_group(1, 4, 1);
    assert(st.min_in_interval(4, 5) == 6);
    st.set_in_group(4, 5, 8);
    assert(st.min_in_interval(4, 5) == 8);
}

void t3() {
    vector<long long int> a{1, 1, 1, 1};
    SegmentTree st(a);
    st.set_in_group(2, 3, 2); // 1122
    st.add_to_group(1, 2, 1); // 1232
    assert(st.min_in_interval(0, 2) == 1);
    assert(st.min_in_interval(1, 3) == 2);
    assert(st.min_in_interval(2, 2) == 3);
}

void t4() {
//    vector<long long int> a{10, 10, 10, 4, 5};
    vector<long long int> a{1, 2, 3, 4, 5};
    SegmentTree st(a);
    st.set_in_group(0, 2, 10); // 10 10 10 4 5
    st.add_to_group(1, 3, 4); // 10 14 14 8 5
    cout << st.min_in_interval(1, 4) << endl;
    cout << st.min_in_interval(0, 3) << endl;
}


void test() {
    t1();
    t2();
    t3();
}

void input() {
    int n;
    cin >> n;
    vector<long long int> a(n);
    long long int tmp;
    for (int i = 0; i < n; ++i) {
        cin >> tmp;
        a.at(i) = tmp;
    }
    SegmentTree st(a);
//    string op;
    int l, r;
    long long int k;
//    for (int row = 0; row < 10; ++row) {
//    for (int row = 0; row < 1e6; ++row) {
//        if (cin.eof()) break;
//        cin >> op >> l >> r;
//        l--;
//        r--;
//        if (op == "min") {
//            cout << st.min_in_interval(l, r) << endl;
//        } else if (op == "set") {
//            cin >> k;
//            st.set_in_group(l, r, k);
//        } else if (op == "add") {
//            cin >> k;
//            st.add_to_group(l, r, k);
//        }
//    }

    string cmd;
    while (cin >> cmd) {
        if (cmd == "min") {
            cin >> l >> r;
        }  else {
            cin >> l >> r >> k;
        }
        l--;
        r--;
        if (cmd == "min") {
            cout << st.min_in_interval(l, r) << endl;
        } else if (cmd == "set") {
            st.set_in_group(l, r, k);
        } else if (cmd == "add") {
            st.add_to_group(l, r, k);
        }

    }
}

int main() {
//    int n = 7;
//    test();
//    t3();
    input();
//    t4();
    return 0;
}
