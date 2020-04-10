#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdint>
#include <assert.h>
#include <string>
#include <cmath>

using namespace std;

//
// Created by Anatolii Burtsev on 2020-03-31.
//

vector<int> generate_a(int n, int a0) {
    long long int tmp = a0;
    vector<int> a(n);
    a.at(0) = a0;
    for (int i = 1; i < n; ++i) {
        tmp = (23 * tmp + 21563) % 16714589;
        a.at(i) = tmp;
    }
    return a;
}

vector<vector<int>> generate_sparse_matrix(int n, const vector<int> &a) {

    vector<vector<int>> matrix(n);
    int m = 1;
    int pow = 0;
    while (m < n) {
        pow++;
        m *= 2;
    }
    m /= 2;

    for (int i = 0; i < n; ++i) {
        matrix.at(i).resize(m);
        matrix.at(i).at(0) = a.at(i);
    }
    int p = 1; // p = 2^(k-1)
    for (int k = 1; k <= pow; ++k) {
        for (int i = 0; i < n; ++i) {
            matrix.at(i).at(k) = min(matrix.at(i).at(k - 1), matrix.at(min(i + p, n - 1)).at(k - 1));
        }

        p *= 2;
    }
    return matrix;
}

int rmq(const vector<vector<int>> &matrix, int a, int b) {
    int l = min(a, b);
    int r = max(a, b);
    if (l == r) {
        return matrix.at(l).at(0);
    }
    int pow = 0;
    int k = 1;
    while (k < r - l + 1) {
        k *= 2;
        pow++;
    }
    pow--;
    k /= 2;
    int m1 = matrix.at(l).at(pow);
    int m2 = matrix.at(r - k + 1).at(pow);
    return min(m1, m2);
}

void process() {
    int n, m, a0;
    int u1, v1;
    cin >> n >> m >> a0 >> u1 >> v1;
    vector<int> a = generate_a(n, a0);
//    for (int i=0; i < a.size(); i++) {
//        cout << i << ":" << a.at(i) << " ";
//    }
//    cout << endl;
    auto mtrx = generate_sparse_matrix(n, a);
    int r;
    r = rmq(mtrx, u1 - 1, v1 - 1);
    long long int u, v;
    u = u1;
    v = v1;
    for (int i = 1; i < m; ++i) {
        u = ((17 * u + 751 + r + 2 * i) % n) + 1;
        v = ((13 * v + 593 + r + 5 * i) % n) + 1;
        r = rmq(mtrx, u-1, v-1);
//        cout << u << " " << v << " : " << r << endl;
    }
    cout << u << " " << v << " " << r << endl;
}


int main() {
    int n = 10000;
    int a0 = 12345;
    const clock_t begin_time = clock();
    vector<int> a = generate_a(n, a0);
//    vector<int> b{3, 2, 3, 1, 2};
    const clock_t middle_time = clock();
    auto mtrx = generate_sparse_matrix(n, a);
    const clock_t end_time = clock();
    cout << "generate a: " << middle_time - begin_time << endl;
    cout << "generate m: " << end_time - middle_time << endl;
//    cout << rmq(mtrx, 4, 3);
//    process();
    return 0;
}
