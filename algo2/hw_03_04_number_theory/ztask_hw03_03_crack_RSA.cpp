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

int gcd(int a, int b, int &x, int &y) {
    if (a == 0) {
        x = 0;
        y = 1;
        return b;
    }
    int x1, y1;
    int d = gcd(b % a, a, x1, y1);
    x = y1 - (b / a) * x1;
    y = x1;
    return d;
}

bool find_any_solution(int a, int b, int c, int &x0, int &y0, int &g) {
    g = gcd(abs(a), abs(b), x0, y0);
    if (c % g != 0)
        return false;
    x0 *= c / g;
    y0 *= c / g;
    if (a < 0) x0 *= -1;
    if (b < 0) y0 *= -1;
    return true;
}

pair<int, int> find_p_q(int n) {
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            return make_pair(i, n / i);
        }
    }
    throw;
}

int fast_find_d(int p, int q, int e) {
    int a = e;
    int b =  - (p - 1) * (q - 1);
    int c = 1;
    int x0, y0, g;
    find_any_solution(a, b, c, x0, y0, g);
    int result = x0;
    if (result < 0) {
        result += abs(b) * (1 + abs(result) / abs(b));
    }
    result %= abs(b);
    return result;
}

int find_d(int p, int q, int e) {
    int m = (p - 1) * (q - 1);
    int d = 1;
    int cur_e = e;
    int res = d * e;
    while (res != 1) {
        d++;
        cur_e += e;
        res = (res + e) % m;
    }
    return d;
}

long long fast_mod_pow(long long base, int power, int mod) {
    long long result = 1;
    while (power > 0) {

        if (power & 1) {
            result = (result * base) % mod;
        }
        base = (base * base) % mod;
        power >>= 1;
    }
    return result;
}

long long mod_pow(int base, int power, int mod) {
    long long result = 1;
    for (int pow = 0; pow < power; ++pow) {
        result = (result * base) % mod;
    }
    return result;

}

long long process(int n, int e, int c) {
    pair<int, int> pq = find_p_q(n);
    int d = fast_find_d(pq.first, pq.second, e);
    return fast_mod_pow(c, d, n);
}

void test() {
    int n = 17 * 37;
    pair<int, int> pq = find_p_q(n);
    assert(pq.first == 17);
    assert(pq.second == 37);
    assert(fast_find_d(2, 5, 5) == 1);
    assert(fast_find_d(2, 5, 3) == 3);
    assert(fast_find_d(11, 13, 113) == 17);
    assert(mod_pow(3, 3, 5) == 2);
    assert(process(143, 113, 41) == 123);
    assert(process(9173503, 3, 4051753) == 111111);

}

int main() {
    int n, e, c;
    cin >> n >> e >> c;
    cout << process(n, e, c);
//    test();
    return 0;
}

