#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdint>
#include <assert.h>
#include <string>
#include <cmath>

using namespace std;

//
// Created by Anatolii on 2020-04-09.
//

// magic Fenwick function
int F(int i) {
    return i & (i + 1);
}

vector<long long int> calculate_T(const vector<long long int> &a, int n) {
    vector<long long int> T(a.size());
    fill(T.begin(), T.end(), 0);
    long long int tmp;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            for (int k = 0; k < n; ++k) {
                tmp = 0;
                for (int i1 = F(i); i1 <= i; ++i1) {
                    for (int j1 = F(j); j1 <= j; ++j1) {
                        for (int k1 = F(k); k1 <= k; ++k1) {
                            tmp += a.at(i1 * n * n + j1 * n + k1);
                        }
                    }
                }
                T.at(i * n * n + j * n + k) = tmp;
            }
        }

    }
    return T;
}

long long int get(int z, int y, int x, const vector<long long int> &T, int n) {
    if (z < 0 || y < 0 || x < 0) return 0;
    long long int res = 0;
    for (int z1 = z; z1 >= 0; z1 = F(z1) - 1) {
        for (int y1 = y; y1 >= 0; y1 = F(y1) - 1) {
            for (int x1 = x; x1 >= 0; x1 = F(x1) - 1) {
                res += T.at(z1 * n * n + y1 * n + x1);
            }
        }
    }
    return res;
}

void add(int x, int y, int z, long long int k, vector<long long int> &T, int n) {
    int z1 = z;
    while (z1 < n) {
        int y1 = y;
        while (y1 < n) {
            int x1 = x;
            while (x1 < n) {
                T.at(z1 * n * n + y1 * n + x1) += k;
                x1 = x1 | (x1 + 1);
            }
            y1 = y1 | (y1 + 1);
        }
        z1 = z1 | (z1 + 1);
    }
}

long long int rsq(int x1, int x2, int y1, int y2, int z1, int z2, const vector<long long int> &T, int n) {
    long long int sum = 0;
    long long int g1, g2, g3, g4, g5, g6, g7, g8;
    g1 = get(z2, y2, x2, T, n);
    g2 = get(z1 - 1, y2, x2, T, n);
    g3 = get(z2, y1 - 1, x2, T, n);
    g4 = get(z2, y2, x1 - 1, T, n);
    g5 = get(z1 - 1, y1 - 1, x2, T, n);
    g6 = get(z1 - 1, y2, x1 - 1, T, n);
    g7 = get(z2, y1 - 1, x1 - 1, T, n);
    g8 = get(z1 - 1, y1 - 1, x1 - 1, T, n);

    sum = g1 - g2 - g3 - g4 + g5 + g6 + g7 - g8;
    return sum;
}

void input() {
    int n;
    cin >> n;
    vector<long long int> a(n * n * n);
    fill(a.begin(), a.end(), 0);
    bool aInitialized = false;
    vector<long long int> T;
    int tmp, x, y, z, x1, x2, y1, y2, z1, z2, k;
    while (cin >> tmp) {
        if (tmp == 3) exit(0);
        if (tmp == 1) {
            cin >> x >> y >> z >> k;
            if (aInitialized) {
                add(x, y, z, k, T, n);
            } else {
                a.at(z * n * n + y * n + x) += k;
            }
        }
        if (tmp == 2) {
            if (!aInitialized) {
                aInitialized = true;
                T = calculate_T(a, n);
            }
            cin >> x1 >> y1 >> z1 >> x2 >> y2 >> z2;
            cout << rsq(x1, x2, y1, y2, z1, z2, T, n) << endl;
        }
    }
}

void t() {
    int n = 3;
    vector<long long int> a(n * n * n);
    fill(a.begin(), a.end(), 0);
    vector<long long int> T = calculate_T(a, n);
    add(1, 1, 1, 1, T, n);
    add(1, 0, 1, 2, T, n);
    add(1, 0, 0, 3, T, n);
    assert(rsq(0, 2, 0, 2, 0, 2, T, n) == 6);
    assert(rsq(0, 1, 0, 1, 0, 1, T, n) == 6);
    assert(rsq(0, 2, 0, 2, 1, 1, T, n) == 3);
}

int main() {
    input();
//    t();
//    int n = 2;
//    vector<long long int> a{1, 3, 2, 4, 5, 7, 6, 8};
//    fill(a.begin(), a.end(), 0);
//    vector<long long int> T = calculate_T(a, n);
//    add(1, 0, 0, 1, T, n);
//    cout << rsq(1, 1, 0, 1, 0, 1, T, n) << endl;
    return 0;
}