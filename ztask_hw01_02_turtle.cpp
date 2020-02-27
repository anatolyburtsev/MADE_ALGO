#include <iostream>
#include <vector>
#include <algorithm>
#include <stdint.h>

using namespace std;

///
// Created by Anatolii Burtsev on 2020-02-26.
//

//ifstream read_file(const string &filename) {
//    ifstream inFile;
//    inFile.open(filename);
//    if (!inFile) {
//        cerr << "Unable to open input file";
//        exit(1);   // call system to stop
//    }
//    return inFile;
//}
//
//void write_to_file(const string &filename, const string value) {
//    ofstream outFile;
//    outFile.open(filename);
//    if (!outFile) {
//        cerr << "Unable to open output file";
//        exit(1);   // call system to stop
//    }
//    outFile << value;
//    outFile.close();
//
//}

void print_field(int n, int m, const vector<int> &field) {
    for (int j = 0; j < m; ++j) {
        for (int i = 0; i < n; ++i) {
            cout << field[j * n + i] << " ";
        }
        cout << endl;
    }
}

void print_field(int n, int m, const vector<char> &field) {
    for (int j = 0; j < m; ++j) {
        for (int i = 0; i < n; ++i) {
            cout << field[j * n + i] << " ";
        }
        cout << endl;
    }
}

void find_most_profitable_path(int n, int m, const vector<int8_t> &field) {
    vector<int> costs(n * m);
    vector<char> path(n * m);
    fill(costs.begin(), costs.end(), INT_MIN);
    for (int i = 0; i < n; ++i) {
        costs[i] = INT_MIN;
    }
    for (int j = 0; j < m; ++j) {
        costs[j * n] = INT_MIN;
    }
    costs[0] = 0;
    costs[1] = 0;
    costs[n] = 0;

    for (int j = 1; j < m; ++j) {
        for (int i = 1; i < n; ++i) {
            int cur_position = j * n + i;
            int upper_position = (j - 1) * n + i;
            int left_position = j * n + i - 1;
            if (costs[upper_position] > costs[left_position]) {
                path[cur_position] = 'D';
                costs[cur_position] = field[cur_position] + costs[upper_position];
            } else {
                path[cur_position] = 'R';
                costs[cur_position] = field[cur_position] + costs[left_position];
            }
        }
    }
    cout << costs[n*m -1] << endl;

    int i = n - 1;
    int j = m - 1;
    vector<char> steps;
    char last_step;
    while (i != 1 || j != 1) {
        last_step = path[j * n + i];
        steps.push_back(last_step);
        if (last_step == 'D') {
            j--;
        } else {
            i--;
        }
    }
    reverse(steps.begin(), steps.end());
    for (char step : steps) {
        cout << step;
    }
    cout << endl;
}

int main() {
    int n, m;
    cin >> m;
    cin >> n;
    n++;
    m++;

    vector<int8_t> field(n * m);
    // fill top and left border with 0
    for (int i = 0; i < n; ++i) {
        field[i] = 0;
    }
    for (int j = 0; j < m; ++j) {
        field[j * n] = 0;
    }

    int tmp;
    for (int j = 1; j < m; ++j) {
        for (int i = 1; i < n; ++i) {
            cin >> tmp;
//            cout << "tmp = " << tmp << endl;
//            tmpc = tmp;
//            cout << "tmpc = " << tmpc << endl;
            field[j * n + i] = tmp;
        }
    }

    find_most_profitable_path(n, m, field);
}
