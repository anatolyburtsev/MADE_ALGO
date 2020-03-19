#include <iostream>
#include <vector>
#include <algorithm>
#include <stdint.h>

using namespace std;
//
// Created by Anatolii on 2020-02-27.
//

void pr(const vector<int> &a) {
    cout << endl;
    for (int i : a) {
        cout << i << " ";
    }
    cout << endl;
}

void find_longest_inc_seq(const vector<int> &a) {
    vector<int> max_inc_seq(a.size());
    vector<int> p(a.size());
    max_inc_seq[0] = 1;
    p[0] = 0;

    for (int i = 1; i < a.size(); ++i) {
        int max_tail_size = 0;
        int max_tail_p = 0;
        for (int j = 0; j < i; ++j) {
            if (a[i] <= a[j]) continue;
            if (max_inc_seq[j] > max_tail_size) {
                max_tail_size = max_inc_seq[j];
                max_tail_p = j;
            }
        }
        max_inc_seq[i] = max_tail_size + 1;
        p[i] = max_tail_p;
    }

    int max_seq = 0;
    int max_seq_p = 0;
    for (int k = 0; k < max_inc_seq.size(); ++k) {
        if (max_inc_seq[k] > max_seq) {
            max_seq = max_inc_seq[k];
            max_seq_p = k;
        }
    }
    cout << max_seq << endl;

    vector<int> path;
    int prev_max_seq_p = max_seq_p;
    while (max_seq_p != 0) {
        path.push_back(max_seq_p);
        prev_max_seq_p = max_seq_p;
        max_seq_p = p[max_seq_p];
    }
    if (a[0] < a[prev_max_seq_p] || a.size() == 1) {
        path.push_back(0);
    }

    reverse(path.begin(), path.end());
    for (int l : path) {
        cout << a[l] << " ";
    }
    cout << endl;
}

int main() {
    int n;
    cin >> n;
    vector<int> nums(n);
    for (int i = 0; i < n; ++i) {
        cin >> nums[i];
    }
    find_longest_inc_seq(nums);
    return 0;
}
