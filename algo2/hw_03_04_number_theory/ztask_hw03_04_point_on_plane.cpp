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
long long gcd(int a, int b, long long &x, long long &y) {
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

bool find_any_solution(int a, int b, int c, long long &x0, long long &y0, long long &g) {
    g = gcd(abs(a), abs(b), x0, y0);
    if (c % g != 0)
        return false;
    x0 *= c / g;
    y0 *= c / g;
    if (a < 0) x0 *= -1;
    if (b < 0) y0 *= -1;
    return true;
}

void process(int a, int b, int c) {
    long long x0, y0, g;
    if (!find_any_solution(a, b, -c, x0, y0, g)) {
        cout << "-1" << endl;
        return;
    }
    cout << x0 << " " << y0 << endl;
}


int main() {
//    int a = 2;
//    int b = 5;
//    int c = 3;
    int a, b, c;
    cin >> a >> b >> c;

    process(a, b, c);
    return 0;
}