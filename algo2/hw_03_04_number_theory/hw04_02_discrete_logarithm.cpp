#include <iostream>
#include <vector>
//#include <algorithm>
#include <cstdint>
#include <assert.h>
#include <string>
#include <unordered_set>
#include <cmath>

using namespace std;

//
// Created by Anatolii Burtsev on 2020-03-19.
//
long long fast_power(long long base, long long power, long long mod) {
    long long result = 1;
    base %= mod;

    while (power > 0) {
        if (power & 1) {
            result = (result * base) % mod;
            if (result < 0) throw;
        }
        power = power >> 1;
        base = (base * base) % mod;
        if (base < 0) throw;
    }
    return result;
}

long long safe_power(long long base, long long power, long long mod) {
    long long result = 1;
    long long plus_part, minus_part;

    base %= mod;
    for (int i = 0; i < power; ++i) {
        result = (result * base);
        if (result < 0) {
            plus_part = (LONG_LONG_MAX - mod) % mod;
            minus_part = (-LONG_LONG_MIN + result) % mod;
            result = plus_part + minus_part;
        }
        if (result < 0) throw;
        result %= mod;
    }
    if (result < 0) throw;
    return result;
}


long long find_discrete_log(long long a, long long b, long long m) {
    long long k = sqrt(m) + 1;

    vector<long long> ba(k);
    std::unordered_set<long long> ba_set(2 * k);

    long long cur_value = (b * a) % m;
    for (long long s = 0; s < k; ++s) {
        ba.at(s) = cur_value;
        ba_set.insert(cur_value);
        cur_value = (cur_value * a) % m;
    }

    long long a_k = safe_power(a, k, m);
    cur_value = a_k;
    for (long long j = 0; j < k; ++j) {
        auto got = ba_set.find(cur_value);
        if (got != ba_set.end()) {
            // match found!
            long long s = -1;
            for (long long i = 0; i < k; ++i) {
                if (ba.at(i) == *got) {
                    s = i + 1;
                    break;
                }
            }
            long long r = j + 1;
            long long res = r * k - s;
            if (s < 0) throw;
            if (res >= m) throw;
            if (res < 0) throw;
            return res;
        }
        cur_value = (cur_value * a_k) % m;
    }
    return -1;

}


int main() {
    long long a, b, m;
    cin >> a >> b >> m;
    cout << find_discrete_log(a, b, m);
    return 0;
}

