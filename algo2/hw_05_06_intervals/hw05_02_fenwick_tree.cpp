#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdint>
#include <assert.h>
#include <string>
#include <cmath>

using namespace std;
//
// Created by Anatolii Burtsev on 2020-04-01.
//

// magic Fenwick function
int F(int i) {
    return i & (i + 1);
}

vector<long long int> calculate_T(const vector<long long int> &a) {
    int n = a.size();
    vector<long long int> T(n);
    long long int tmp;
    for (int i = 0; i < n; ++i) {
        tmp = 0;
        for (int j = F(i); j <= i; ++j) {
            tmp += a.at(j);
        }
        T.at(i) = tmp;
    }
    return T;
}

long long int get(int i, const vector<long long int> &T) {
    long long int res = 0;
    while (i >= 0) {
        res += T.at(i);
        i = F(i) - 1;
    }
    return res;
}

void add(int i, long long int x, vector<long long int> &T) {
    int n = T.size();
    int j = i;
    while (j < n) {
        T.at(j) += x;
        j = j | (j + 1);
    }
}

void set(int i, long long int x, vector<long long int> &T, vector<long long int> &a) {
    long long int d = x - a.at(i);
    a.at(i) = x;
    add(i, d, T);
}

long long int rsq(int l, int r, const vector<long long int> &T) {
    if (l == 0) {
        return get(r, T);
    } else {
        return get(r, T) - get(l - 1, T);
    }

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
    vector<long long int> T = calculate_T(a);
    string cmd;
    long long int b, c;
    while (cin >> cmd) {
        cin >> b >> c;
        b--;
        if (cmd == "sum") {
            c--;
            cout << rsq(b, c, T) << endl;
        } else if (cmd == "set") {
            set(b, c, T, a);
        } else {
            throw;
        }
    }
}

void t2() {
    vector<long long int> a{3, 1, 5, 6, 2, 10, 17};
    vector<long long int> T = calculate_T(a);
    assert(rsq(0, 3, T) == 15);
    assert(rsq(2, 6, T) == 40);
    assert(rsq(2, 4, T) == 13);
}

void t3() {
    vector<long long int> a{1, 1, 1, 1};
    vector<long long int> T = calculate_T(a);
    set(1, 2, T, a);
    set(2, 3, T, a);
    assert(rsq(0, 2, T) == 6);
    set(2, 0, T, a);
    assert(rsq(0, 2, T) == 3);
}

void test() {
    assert(F(5) == 4);
    assert(F(6) == 6);
    assert(F(7) == 0);
    t2();
    t3();
}

int main() {
//    test();
    input();

    return 0;
}
