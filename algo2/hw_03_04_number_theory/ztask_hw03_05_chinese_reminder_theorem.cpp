#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdint>
#include <assert.h>
#include <string>
#include <cmath>

using namespace std;

//
// Created by Anatolii Burtsev on 2020-03-14.
//

long long gcd(long long a, long long b, long long &x, long long &y) {
    if (a == 0) {
        x = 0;
        y = 1;
        return b;
    }
    long long x1, y1;
    long long d = gcd(b % a, a, x1, y1);
    x = y1 - (b / a) * x1;
    y = x1;
    return d;
}

bool find_any_solution(long long a, long long b, long long c, long long &x0, long long &y0, long long &g) {
    g = gcd(abs(a), abs(b), x0, y0);
    if (c % g != 0)
        return false;
    x0 *= c / g;
    y0 *= c / g;
    if (a < 0) x0 *= -1;
    if (b < 0) y0 *= -1;
    return true;
}

long long process(int a1, int a2, int m1, int m2) {
    // solve for case 1, 0
    long long p1, q1, g1;
    find_any_solution(-m1, m2, 1, p1, q1, g1);
    if (q1 < 0) {
        q1 += (m1 * (1 + -q1 / m1));
    }

    // solve for case 0, 1
    long long p2, q2, g2;
    find_any_solution(m1, -m2, 1, p2, q2, g2);
    if (p2 < 0) {
        p2 += (m2 * (1 + -p2 / m2));
    }

    // combine solutions
    long long x_1 = a1 * q1;
    long long x_2 = a2 * p2;

    long long k1 = x_1 / m1;
    long long k2 = x_2 / m2;
    long long k = min(k1, k2);
    x_1 -= k * m1;
    x_2 -= k * m2;

    long long part1 = m2 * x_1;
    long long part2 = m1 * x_2;
    // what we  know:
    // a1 > 0; q1 > 0; a2 > 0; p2 > 0
    // x_1 > 0; x_2 > 0
    // m1 > 0; m2 > 0

    // part1  < 0 || part2 < 0
    // m1*m2 < 0
    if (part1 < 0 || part2 < 0) throw;

//    k1 = (part1 / m1) / m2;
//    k2 = (part2 / m1 ) / m2;
//    k = min(k1, k2);
//    part1 -= k * m;
//    part2 -= k * m;

    long long x = part1 + part2;
//    if (part1 > 0 && part2 > 0 && (x < part1 || x < part2)) throw;
    if (part1 > 0 && part2 > 0 && x < 0) {
        long long plus_part = LONG_LONG_MAX - m1 * m2;
        long long minus_part = x - LONG_LONG_MIN;
        assert(minus_part > 0);
        assert(plus_part > 0);
        return (plus_part + minus_part) ;
    }
//    long long x = m2 * x_1 + m1 * x_2;
//    if (k > 0 && x < 0 && x_1 > 0 && x_2 > 0) throw;

//    long long x = a1 * m2 * q1 + a2 * m1 * p2;

    if (x < 0) {
        x += (m1 * m2 * (1 + -1 * x / (m1 * m2)));
    }

    x %= (m1 * m2);
    return x;
}

//// returns x where (a * x) % b == 1
//long long mul_inv(long long a, long long b)
//{
//    long long b0 = b, t, q;
//    long long x0 = 0, x1 = 1;
//    if (b == 1) return 1;
//    while (a > 1) {
//        q = a / b;
//        t = b, b = a % b, a = t;
//        t = x0, x0 = x1 - q * x0, x1 = t;
//    }
//    if (x1 < 0) x1 += b0;
//    return x1;
//}
//
//long long chinese_remainder(long long *n, long long *a, long long len)
//{
//    long long p, i, prod = 1, sum = 0;
//
//    for (i = 0; i < len; i++) prod *= n[i];
//
//    for (i = 0; i < len; i++) {
//        p = prod / n[i];
//        sum += a[i] * mul_inv(p, n[i]) * p;
//    }
//
//    return sum % prod;
//}
//
//long long process2(int a1, int a2, int m1, int m2) {
//    long long n[] = {m1, m2};
//    long long a[] = {a1, a2};
//    return chinese_remainder(n, a, 2);
//}

void test() {
    assert(process(3, 1, 5, 3) == 13);
    assert(process(1, 0, 2, 3) == 3);
    assert(process(3, 2, 5, 9) == 38);
    assert(process(1, 1, 3, 2) == 1);
}

void input() {
    long long n, a1, a2, m1, m2;
    cin >> n;
    for (int i = 0; i < n; ++i) {
        cin >> a1 >> a2 >> m1 >> m2;
        cout << process(a1, a2, m1, m2) << endl;
    }
}

int main() {
    input();
//    test();
//    int a = 1000000000;
//    int b = 1000000000;
//    long long c = a * b;
//    long long d = LONG_LONG_MAX / a / b;
//    cout << c <<endl;
//    cout << d <<endl;
    return 0;
}
