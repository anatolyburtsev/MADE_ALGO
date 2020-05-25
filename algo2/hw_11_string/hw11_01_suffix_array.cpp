#include <iostream>
#include <fstream>
#include <utility>
#include <vector>
#include <algorithm>
#include <cstdint>
#include <assert.h>
#include <string>
#include <cmath>
#include <ctime>
#include <queue>

using namespace std;

//
// Created by Anatolii on 2020-05-23.

//A. Suffix array
//time limit per test2 seconds
//        memory limit per test512 megabytes
//inputstandard input
//outputstandard output
//Build a suffix array for a given string s, for each two adjacent suffixes find the length of longest common prefix.
//
//Input
//        First line holds a single string s (1 ≤ |s| ≤ 400 000). String consists of small english letters.
//
//Output
//        In first line output |s| distinct integers — numbers of first symbols of s suffixes in a way, that according suffixes will be lexicogrpahically sorted in acsending order. In second line output |s| - 1 integers — lengths of longest common prefixes.
//
//Example
//        inputCopy
//ababb
//        outputCopy
//1 3 5 2 4
//2 0 1 1
//

// task 2
//C. Количество подстрок
//ограничение по времени на тест2 секунды
//ограничение по памяти на тест512 мегабайт
//вводстандартный ввод
//выводстандартный вывод
//Вычислите количество различных подстрок строки s.
//
//Входные данные
//Единственная строка входного файла содержит строку s (1 ≤ |s| ≤ 400 000). Строка состоит из строчных латинских букв.
//
//Выходные данные
//Выведите одно число — ответ на задачу.
//
//Пример
//        входные данныеCopy
//        ababb
//выходные данныеCopy
//11


class SufArr {
public:
    SufArr(string str_) : str(std::move(str_)) {};
    char EOS = '$';
    int n;
    int orig_length;
    string str;
    vector<int> c;
    vector<int> p;
    vector<int> pos;
    vector<int> lcp;

    void prepare();

    void calc_lcp();

    void print_answer();

    void print_answer_task2();

private:

    void init();

    bool step(int k);

    void count_sort(vector<pair<pair<int, int>, int>> &arr);

    void round_str();

    void cut_p();
};

void SufArr::init() {
    orig_length = str.length();
    round_str();
    n = str.length();
    vector<pair<char, int>> s;
    s.reserve(n);
    p.reserve(n);
    c.reserve(n);
    pos.reserve(n);
    for (int i = 0; i < str.length(); ++i) {
        s.emplace_back(str.at(i), i);
    }
    sort(s.begin(), s.end());
    for (int i = 0; i < n; ++i) {
        p.push_back(s.at(i).second);
    }

    int cur_class = 0;
    char cur_smbl = s.at(0).first;
    for (int i = 0; i < n; ++i) {
        if (s.at(i).first != cur_smbl) {
            cur_smbl = s.at(i).first;
            cur_class++;
        }
        c.push_back(cur_class);
    }

    for (int i = 0; i < n; ++i) {
        pos.push_back(-1);
    }
    for (int i = 0; i < n; ++i) {
        pos.at(p.at(i)) = c.at(i);
    }
}

bool SufArr::step(int k) {
    int step_size = 1 << k;
    vector<pair<pair<int, int>, int>> a;
    a.reserve(n);
    for (int i = 0; i < n; ++i) {
        a.emplace_back(make_pair(pos.at((p.at(i) - step_size + n) % n), c.at(i)), p.at(i));
    }
    count_sort(a);
    for (int i = 0; i < n; ++i) {
        p.at(i) = (a.at(i).second - step_size + n) % n;
    }

    int cur_class = 0;
    pair<int, int> cur_smbl = a.at(0).first;
    for (int i = 0; i < n; ++i) {
        if (a.at(i).first != cur_smbl) {
            cur_smbl = a.at(i).first;
            cur_class++;
        }
        c.at(i) = cur_class;
    }

    for (int i = 0; i < n; ++i) {
        pos.at(p.at(i)) = c.at(i);
    }

    return c.at(n - 1) == n - 1;
}

void SufArr::prepare() {
    init();
    int i = 0;
    while (!step(i)) {
        i++;
    }
    cut_p();
}

void SufArr::count_sort(vector<pair<pair<int, int>, int>> &arr) {
    vector<int> count(n);
    vector<pair<pair<int, int>, int>> output(n);
    fill(count.begin(), count.end(), 0);
    for (int i = 0; i < n; ++i) {
        count.at(arr.at(i).first.first)++;
    }
    for (int i = 1; i < n; ++i) {
        count.at(i) += count.at(i - 1);
    }

    for (int i = n - 1; i >= 0; --i) {
        output.at(count.at(arr.at(i).first.first) - 1) = arr.at(i);
        --count.at(arr.at(i).first.first);
    }

    for (int i = 0; i < n; ++i) {
        arr.at(i) = output.at(i);
    }
}

void SufArr::round_str() {
    str.push_back(EOS);
    int l = str.length();
    int k = 1;
    while (k < l) {
        k *= 2;
    }
    for (int i = l; i < k; ++i) {
        str.push_back(EOS);
    }
}

void SufArr::cut_p() {
    int i;
    for (i = 0; i < n; ++i) {
        if (p.at(i) == n - 1) {
            break;
        }
    }
    vector<int> q;
    q.reserve(n - i);
    for (int j = 1; j < n - i; ++j) {
        q.push_back(p.at(i + j));
    }
    p = q;
    n = orig_length;

    vector<int> pos2;
    pos2.reserve(n - i);
    for (int j = 0; j < n; ++j) {
        pos2.push_back(pos.at(j) - i - 1);
    }
    pos = pos2;
}

void SufArr::calc_lcp() {
    for (int i = 0; i < n; ++i) {
        lcp.push_back(-1);
    }
    int prev = 0;
    int cur;
    for (int i = 0; i < n; ++i) {
        if (pos.at(i) == 0) {
            lcp.at(0) = 0;
            continue;
        }
        int j = p.at(pos.at(i) - 1);
        cur = prev;
        int q = 0;
        while (str.at(i + cur) == str.at(j + cur) && (str.at(i + cur) != EOS && str.at(j + cur) != EOS)) {
            q++;
            cur++;
        }
        lcp.at(pos.at(i)) = cur;
        prev = max(cur - 1, 0);
    }
}

void SufArr::print_answer() {
    for (int i = 0; i < n; ++i) {
        cout << p.at(i) + 1 << " ";
    }
    cout << endl;
    for (int i = 1; i < n; ++i) {
        cout << lcp.at(i) << " ";
    }
    cout << endl;
}

void SufArr::print_answer_task2() {
    long long l = n;
    l = l * (l+1) / 2;
    for (int i = 0; i < n; ++i) {
        l -= lcp.at(i);
    }
    cout << l << endl;
}


int main() {
    // fast input output c++ magic trick
    std::ios::sync_with_stdio(false), std::cin.tie(0), std::cout.tie(0);

    string s;
    cin >> s;
    SufArr sa(s);
    sa.prepare();
    sa.calc_lcp();
    sa.print_answer_task2();
    return 0;
}
