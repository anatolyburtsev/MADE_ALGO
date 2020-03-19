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

class MiniFactorize {
public:
    MiniFactorize(int devider, int power, int restPart, int n) :
            devider(devider), power(power), rest_part(restPart),
            n(n) {}

    int n;
    int devider;
    int power;
    int rest_part;
};

MiniFactorize miniFactorize(int n, const vector<int> &primes) {
    vector<int> deviders;
    int next_devider = find_next_devider(n, primes);
    if (next_devider < 0 || next_devider == n) {
        return {-1, -1, n, n};
    }
    int min_devider_power = 1;
    int rest_part = n;
    rest_part /= next_devider;
    while (rest_part % next_devider == 0) {
        min_devider_power++;
        rest_part /= next_devider;
    }
    return {next_devider, min_devider_power, rest_part, n};
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

int func1(const MiniFactorize miniFactorize) {
    if (miniFactorize.devider > 0) {
        return miniFactorize.devider;
    } else {
        // prime
        return miniFactorize.rest_part;
    }
}

int func2(const MiniFactorize miniFactorize, vector<int> *s0s) {
    int result;
    if (miniFactorize.devider < 0) {
        result = 2;
    } else {
        result = s0s->at(miniFactorize.rest_part) * (miniFactorize.power + 1);
    }
    s0s->at(miniFactorize.n) = result;
    return result;
}

pair<long long, long long> func34(const MiniFactorize miniFactorize, vector<long long> *s1s, vector<long long> *fis) {
    long long result3;
    long long result4;

    if (miniFactorize.devider < 0) {
        result3 = miniFactorize.n + 1;
        result4 = miniFactorize.n - 1;
    } else {
        int p = miniFactorize.devider;
        int pow = miniFactorize.power;
        long long p_k_1 = fast_power(p, pow - 1);
        long long p_k = p_k_1 * p;
        long long p_k_plus_1 = p_k * p;
        result3 = s1s->at(miniFactorize.rest_part) * (p_k_plus_1 - 1) / (p - 1);
        result4 = fis->at(miniFactorize.rest_part) * (p_k - p_k_1);
    }
    s1s->at(miniFactorize.n) = result3;
    fis->at(miniFactorize.n) = result4;

    return make_pair(result3, result4);
}

void process(int n) {
    vector<int> sieve = calcSieve(3200);
    vector<int> *s0s = new vector<int>(n + 1);
    vector<long long> *s1s = new vector<long long>(n + 1);
    vector<long long> *fis = new vector<long long>(n + 1);
    s0s->at(1) = 1;
    s1s->at(1) = 1;
    fis->at(1) = 1;
    long long d = 0;
    long long s_0 = 1;
    long long s_1 = 1;
    long long fi = 1;
    for (int i = 2; i < n + 1; ++i) {
        MiniFactorize mf = miniFactorize(i, sieve);
        d += func1(mf);
        s_0 += func2(mf, s0s);
        pair<long long, long long> res34 = func34(mf, s1s, fis);
        s_1 += res34.first;
        fi += res34.second;
    }
    cout << d << " " << s_0 << " " << s_1 << " " << fi << endl;
}

int main() {
    int n ;
    cin >> n;
    process(n);
    return 0;
}

