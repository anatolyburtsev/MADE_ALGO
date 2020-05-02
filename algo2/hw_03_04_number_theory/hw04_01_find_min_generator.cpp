#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdint>
#include <assert.h>
#include <string>
#include <cmath>

using namespace std;

//
// Created by Anatolii Burtsev on 2020-03-18.
//

int find_min_divider(int n) {
    for (int i = 2; i * i <= n; ++i) {
        if (n % i == 0) {
            return i;
        }
    }
    return -1;
}

vector<int> find_uniq_dividers(int n) {
    vector<int> dividers;
    int next_divider = -1;
    int last_divider = -1;
    while (n != 1) {
        last_divider = next_divider;
        next_divider = find_min_divider(n);
        if (next_divider == last_divider && next_divider > 0) {
            n /= next_divider;
            continue;
        } else if (next_divider > 0) {
            dividers.push_back(next_divider);
            n /= next_divider;
        } else {
            if (n != last_divider) {
                dividers.push_back(n);
            }
            break;
        }
    }
    return dividers;
}

int fast_power(long long base, int power, int mod) {
    long long result = 1;
    base %= mod;

    while (power > 0) {
        if (power & 1) {
            result = (result * base) % mod;
        }
        power = power >> 1;
        base = (base * base) % mod;
    }
    return result;
}

int find_generator(int p) {
    vector<int> uniq_dividers = find_uniq_dividers(p - 1);
    vector<int> uniq_powers;
    bool is_generator;
    uniq_powers.reserve(uniq_dividers.size());
    for (int q: uniq_dividers) {
        uniq_powers.push_back((p - 1) / q);
    }
    reverse(uniq_powers.begin(), uniq_powers.end());

    for (int g = 2; g < p; ++g) {
        is_generator = true;
        for (int power : uniq_powers) {
            if (power < 3) {
                continue;
            }
            int g_power = fast_power(g, power, p);
//            cout << "g = " << g << " power = " << power << " g^power = " << g_power << endl;
            if (g_power == 1) {
                is_generator = false;
                break;
            }
        }
        if (is_generator) return g;
    }
    throw;
    return -1;
}

void test() {
    assert(find_min_divider(10) == 2);
    assert(find_min_divider(27) == 3);
    assert(find_min_divider(7) == -1);
    vector<int> v1{2};
    assert(find_uniq_dividers(8) == v1);
    vector<int> v2{2, 3};
    assert(find_uniq_dividers(24) == v2);
    vector<int> v3{7};
    assert(find_uniq_dividers(7) == v3);
    assert(find_generator(127) == 3);
    assert(find_generator(239) == 7);
    assert(find_generator(3) == 2);
}

void input() {
    int n;
    cin >> n;
    cout << find_generator(n);
}

int main() {
//    test();
    input();
    return 0;
}
