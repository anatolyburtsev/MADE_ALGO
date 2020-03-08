#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdint>
#include <assert.h>
#include <string>

using namespace std;

//
// Created by Anatolii on 2020-02-28.
//

void print_field(int n, int m, const vector<int> &field) {
    for (int j = 0; j < m; ++j) {
        for (int i = 0; i < n; ++i) {
            cout << field[j * n + i] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

int diag_back(int n, int i, int j) {
    return i - 1 + (j - 1) * n;
}

int left_back(int n, int i, int j) {
    return i - 1 + j * n;
}

int up_back(int n, int i, int j) {
    return i + (j - 1) * n;
}

int find_levenstein_distance(const string &a, const string &b) {
    int n = a.size() + 1;
    int m = b.size() + 1;
    if (n == 1) return m - 1;
    if (m == 1) return n - 1;

    vector<int> c(n * m);
    for (int i = 0; i < n; ++i) {
        c[i] = INT_MAX;
    }
    for (int j = 0; j < m; ++j) {
        c[j * n] = INT_MAX;
    }
    c[0] = 0;


    for (int j = 1; j < m; ++j) {
        for (int i = 1; i < n; ++i) {
            if (a[i - 1] == b[j - 1]) {
                if ((i == 1 && j == 1) || (i > 1 && j > 1)) {
                    c[i + j * n] = c[diag_back(n, i, j)];
                } else if (i == 1) {
                    int additional = 1;
                    if ((c[up_back(n, i, j)] == j - 1) && (a[i-1] == b[j-1])) {
                        additional = 0;
                    }
                    c[i + j * n] = c[up_back(n, i, j)] + additional;
                } else if (j == 1) {
                    int additional = 1;
                    if ((c[left_back(n, i, j)] == i - 1) && (a[i-1] == b[j-1])) {
                        additional = 0;
                    }
                    c[i + j * n] = c[left_back(n, i, j)] + additional;
                }
            } else {
                c[i + j * n] = min(c[diag_back(n, i, j)], min(c[left_back(n, i, j)], c[up_back(n, i, j)])) + 1;
            }
//            cout << "i = " << i << " j = " << j << endl;
//            print_field(n, m, c);
        }
    }


    return c[n * m - 1];
}

void test() {
    assert(find_levenstein_distance("AC", "ADC") == 1);
    assert(find_levenstein_distance("AC", "AC") == 0);
    assert(find_levenstein_distance("AC", "BDC") == 2);
    assert(find_levenstein_distance("ADC", "ABC") == 1);
    assert(find_levenstein_distance("ADCE", "ABCF") == 2);
    assert(find_levenstein_distance("AAA", "AAA") == 0);
    assert(find_levenstein_distance("BAA", "BAAA") == 1);
    assert(find_levenstein_distance("BA", "BAAA") == 2);
    assert(find_levenstein_distance("AAB", "AAAB") == 1);
    assert(find_levenstein_distance("A", "B") == 1);
    assert(find_levenstein_distance("A", "A") == 0);
    assert(find_levenstein_distance("A", "AAA") == 2);
    assert(find_levenstein_distance("A", "AA") == 1);
    assert(find_levenstein_distance("ABA", "BA") == 1);
    assert(find_levenstein_distance("AAABAAA", "BABA") == 4);
    assert(find_levenstein_distance("AAABAAA", "AAAAAA") == 1);
    assert(find_levenstein_distance("AAABAAA", "AAAAA") == 2);
    assert(find_levenstein_distance("C", "CBBC") == 3);
}

int main() {
    string a, b;
    cin >> a;
    cin >> b;
    cout << find_levenstein_distance(a, b);
//    test();
    return 0;
}
