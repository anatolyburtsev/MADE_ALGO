#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdint>
#include <assert.h>
#include <string>
#include <cmath>

using namespace std;


//
// Created by Anatolii Burtsev on 2020-03-28.
//
long long int A_MOD = ((1 << 16) - 1);

class SmartArray {
public:
    SmartArray(int n, long x, long y, long a_0) {
        prefix_sums = new vector<long long>(n);
        prefix_sums->at(0) = a_0;
        long long int a = a_0;
        long long int last_a = a_0;
        for (int i = 1; i < n; ++i) {
            a = (x * a + y) & A_MOD;
            if (a < 0) throw; // no
            last_a += a;
            prefix_sums->at(i) = last_a;
        }
    };

    ~SmartArray() {
        delete prefix_sums;
    }

    long long rsq(int c1, int c2) {
        int left_border = min(c1, c2);
        int right_border = max(c1, c2);
        if (left_border == 0) {
            return prefix_sums->at(right_border);
        } else {
            return prefix_sums->at(right_border) - prefix_sums->at(left_border - 1);
        }
    }

private:
    vector<long long> *prefix_sums;
};

long long B_MOD = ((1 << 30) - 1);

class SmartB {
public:
    SmartB(long _z, long _t, long _b_0) : z(_z), t(_t), b(_b_0), last_i(0) {};

    long get_by_index(int i) {
        unsigned long long int tmp = 0;
        for (int j = this->last_i; j < i; ++j) {
            this->b = (this->z * this->b + this->t) & B_MOD;
        }
        this->last_i = i;
        return this->b;
    }

private:
    long long z, t, b;
    int last_i;
};

long long int process(int n, long x, long y, long a0, int m, long z, long t, long b0) {
    SmartArray sa(n, x, y, a0);
    SmartB b(z, t, b0);

    long long int sum = 0;
    long long int b1, b2;
    for (int i = 0; i < m; ++i) {
        b1 = b.get_by_index(2 * i) % n;
        b2 = b.get_by_index(2 * i + 1) % n;
        if (b1 < 0) b1 += n;
        if (b2 < 0) b2 += n;
        sum += sa.rsq(b1, b2);
    }
    return sum;
}

void test() {
    SmartArray sa(3, 1, 2, 3);
    assert(sa.rsq(0, 0) == 3);
    assert(sa.rsq(0, 1) == 8);
    assert(sa.rsq(2, 1) == 12);
    assert(process(3, 1, 2, 3, 3, 1, -1, 4) == 23);
    assert(process(10000000, 5345678, 1776543, 3765437, 1000000, 39876543, 10654347, 58765467) == 6103399168622348);
    assert(process(10000000, 1, 2, 3, 10000000, 2, 1, 3) == 560697549824);
}

void input() {
    int n, m;
    long x, y, z, t, a0, b0;
    cin >> n >> x >> y >> a0 >> m >> z >> t >> b0;
    cout << process(n, x, y, a0, m, z, t, b0) << endl;
}

int main() {
//    test();
    input();
//    cout << process(3, 1, 2, 3, 3, 1, -1, 4);
//    cout << process(10000000, 1, 2, 3, 10000000, 2, 1, 3);
    return 0;
}

