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

int main() {
    vector<int> sieve = calcSieve(1000);
    // fast input output c++ magic trick
    std::ios::sync_with_stdio(false), std::cin.tie(0), std::cout.tie(0);
    int n;
    cin >> n;
    int num;
    for (int i = 0; i < n; ++i) {
        cin >> num;
        vector<int> d = factorize(num, sieve);
        for (int j = 0; j < d.size(); ++j) {
            cout << d.at(j) << " ";
        }
        cout << "\n";
    }

    return 0;
}

