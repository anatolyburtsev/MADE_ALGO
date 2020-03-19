#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdint>
#include <assert.h>
#include <string>
#include <cmath>

using namespace std;

//
// Created by Anatolii on 2020-03-11.
//
int find_next_devider(int n, const vector<int> &primes) {
    int limit = sqrt(n);
    for (int prime : primes) {
        if (n % prime == 0) {
            return prime;
        }
        if (prime > limit + 1) {
            return -1;
        }
    }
    return -1;
}

vector<int> factorize(int n, const vector<int> &primes) {
    vector<int> deviders;
    int next_devider;
    while (n != 1) {
        next_devider = find_next_devider(n, primes);
        if (next_devider > 0) {
            deviders.push_back(next_devider);
            n /= next_devider;
        } else {
            deviders.push_back(n);
            break;
        }
    }
    return deviders;
}

vector<int> calcSieve(int max_n) {
    vector<bool> is_prime_number(max_n);
    fill(is_prime_number.begin(), is_prime_number.end(), true);

    int prime;
    int j = 0;
    for (int i = 2; i < max_n; ++i) {
        if (!is_prime_number.at(i)) continue;
        prime = i;
        j = 2 * prime;
        while (j < max_n) {
            is_prime_number.at(j) = false;
            j += prime;
        }
    }

    vector<int> result;
    for (int i = 2; i < max_n; ++i) {
        if (is_prime_number.at(i)) {
            result.push_back(i);
        }
    }
    return result;
}

// list of pairs (devider, power)
vector<pair<int, int>> reformat_deviders(const vector<int> &deviders) {
    vector<pair<int, int>> result;
    int cur_devider = deviders.at(0);
    int last_devider = cur_devider;
    int devider_power = 1;
    for (int i = 1; i < deviders.size(); ++i) {
        cur_devider = deviders.at(i);
        if (cur_devider == last_devider) {
            devider_power++;
        } else {
            result.emplace_back(last_devider, devider_power);
            last_devider = cur_devider;
            devider_power = 1;
        }
    }
    result.emplace_back(last_devider, devider_power);

    return result;
}


long long fast_power(int base, int power) {
    long long result = 1;
    while (power > 0) {

        if (power & 1) {
            result = (result * base);
        }
        base = (base * base);
        power >>= 1;
    }
    return result;
}

int func1(const vector<int> &deviders) {
    return deviders.at(0);
}

int func2(const vector<pair<int, int>> &grouped_deviders) {
    int result = 1;
    for (int i = 0; i < grouped_deviders.size(); ++i) {
        int k = grouped_deviders.at(i).second;
        result *= (k + 1);
    }
    return result;
}

pair<long long, long long> func34(const vector<pair<int, int>> &grouped_deviders) {
    long long result3 = 1;
    long long result4 = 1;

    for (const auto & grouped_devider : grouped_deviders) {
        int p = grouped_devider.first;
        int pow = grouped_devider.second;

        long long p_k_1 = fast_power(p, pow - 1);
        long long p_k = p_k_1 * p;
        long long p_k_plus_1 = p_k * p;
        result4 *= (p_k - p_k_1);
        result3 *= (p_k_plus_1 - 1) / (p - 1);
    }
    return make_pair(result3, result4);
}

void process(int n) {
    vector<int> sieve = calcSieve(3200);
    long long d = 0;
    long long s_0 = 1;
    long long s_1 = 1;
    long long fi = 1;
    for (int i = 2; i < n + 1; ++i) {
        vector<int> deviders = factorize(i, sieve);
        vector<pair<int, int>> grouped_deviders = reformat_deviders(deviders);
        d += func1(deviders);
        s_0 += func2(grouped_deviders);
        pair<long long, long long> res34 = func34(grouped_deviders);
        s_1 += res34.first;
        fi += res34.second;
    }
    cout << d << " " << s_0 << " " << s_1 << " " << fi << endl;
}

int main() {
    int n;
    cin >> n;
    process(n);

//    for (int i = 1; i < 11; ++i) {
//        process(i);
//    }

    return 0;
}

