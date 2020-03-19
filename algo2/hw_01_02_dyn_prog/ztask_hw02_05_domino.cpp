#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdint>
#include <assert.h>
#include <string>

using namespace std;
//
// Created by Burtsev, Anatolii on 2020-03-08.
//

class Board {
public:
    Board(int n, int m) {
        this->n = n;
        this->m = m;
        cells.resize(n);
        for (int i = 0; i < n; ++i) {
            cells.at(i).resize(m);
        }
        dp.resize(n * m + 1);
        for (int i = 0; i < n * m + 1; ++i) {
            dp.at(i).resize(1 << n);
            fill(dp.at(i).begin(), dp.at(i).end(), 0);
        }
    }

    void add_cell(int i, char value);

    bool isCellBlocked(int i, int j);

    long long int calc();

private:
    int m;
    int n;
    vector<vector<char>> cells;
    vector<vector<long long int>> dp;
};

void Board::add_cell(int i, char value) {
    int row = i / m;
    int col = i % m;
    cells.at(row).at(col) = value;
}


long long int Board::calc() {
    for (int mask = 0; mask < 1 << n; ++mask) {
        dp.at(0).at(mask) = 0;
    }
    dp.at(0).at(0) = 1;

    for (int x = 0; x < n * m; ++x) {
        int i = x % n; // current row
        int j = x / n; // current column
        for (int mask = 0; mask < 1 << n; ++mask) {
            // do nothing
            if ((mask & 1) == 1 || isCellBlocked(i, j)) {
                dp[x + 1][mask >> 1] += dp[x][mask];
            }

            // put horizontal
            if ((mask & 1) == 0 && !isCellBlocked(i, j + 1) && !isCellBlocked(i, j)) {
                int mask2 = (mask >> 1) + (1 << (n - 1));
                dp[x + 1][mask2] += dp[x][mask];
            }

            // put vertical
            if (((mask & 1) == 0) && ((mask & 2) == 0) && (i != n - 1) && !isCellBlocked(i + 1, j) &&
                !isCellBlocked(i, j)) {
                int mask3 = (mask >> 1) + 1;
                dp[x + 1][mask3] += dp[x][mask];
            }

        }
    }

    return dp[n * m][0];
}

bool Board::isCellBlocked(int i, int j) {
    if (i >= n || j >= m) return false;
    return cells.at(i).at(j) == 'X';
}


int getResult(int n, int m, string field) {
    Board b(n, m);
    for (int i = 0; i < field.size(); ++i) {
        b.add_cell(i, field.at(i));
    }
    return b.calc();
}

void test() {
    assert(getResult(2, 2, "....") == 2);
    assert(getResult(2, 3, "....") == 3);
    assert(getResult(2, 3, "X..X..") == 2);
    assert(getResult(3, 3, "........X") == 4);
}

void input() {
    int n, m;
    cin >> n;
    cin >> m;
    Board b(n, m);
    char tmp;
    for (int i = 0; i < n * m; ++i) {
        cin >> tmp;
        b.add_cell(i, tmp);
    }
    cout << b.calc();
}


int main() {
//    test();
    input();

    return 0;
}
