#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdint>
#include <assert.h>
#include <string>

using namespace std;
//
// Created by Anatolii on 2020-03-06.
//


class Graph {
public:
    Graph(int n) {
        this->n = n;
        this->nodes = new vector<int>(n * n);
        this->dp = new vector<int>(1 << n);
    }

    ~Graph() {
        this->nodes->clear();
    }

    void addNode(int index, char letter) {
        if (letter == 'N') {
            this->nodes->at(index) = 0;
        } else {
            this->nodes->at(index) = 1;
        }
    }

    bool isEdgeExist(int i, int j);

    int findMaxPairs();

    static int getBit(int mask, int i) {
        return (mask >> i) & 1;
    }

private:
    int n;
    vector<int> *nodes;
    vector<int> *dp;

};

bool Graph::isEdgeExist(int i, int j) {
    if (i == j) return false;
    return nodes->at(i * n + j) > 0;
}

int Graph::findMaxPairs() {
    dp->at(0) = 0;
    for (int mask = 0; mask < (1 << n); ++mask) {
        for (int i = 0; i < n; ++i) {
            if (getBit(mask, i) == 1) {
                dp->at(mask) = max(dp->at(mask), dp->at(mask - (1 << i)));
                for (int j = 0; j < n; ++j) {
                    if (getBit(mask, j) && isEdgeExist(i, j)) {
                        dp->at(mask) = max(dp->at(mask), dp->at(mask - (1 << i) - (1 << j)) + 1);
                    }
                }
            }
        }

    }

    return dp->at((1 << n) - 1);
}

int getResult(int n, string input) {
    Graph g(n);
    for (int i = 0; i < n * n; ++i) {
        g.addNode(i, input[i]);
    }
    return g.findMaxPairs();
}

void test() {
    assert(getResult(1, "N") == 0);
    assert(getResult(2, "NNNN") == 0);
    assert(getResult(2, "NYYN") == 1);
    assert(getResult(3, "NYNYNYNYN") == 1);
    assert(getResult(3, "NYYYNYYYN") == 1);
    assert(getResult(3, "NNNNNYNYN") == 1);
    assert(getResult(4, "NNYNNNYNYYNYNNYN") == 1);
    assert(getResult(4, "NYNNYNYNNYNYNNYN") == 2);
//    assert(getResult(4, "YYYYYYYYYYYYYYYY") == 2);
    assert(getResult(4, "NNYNNNNYYNNNNYNN") == 2);
    assert(getResult(4, "NYNYYNYNNYNYYNYN") == 2); // square
    assert(getResult(5, "NYYYYYNNNNYNNNYYNNNYYNYYN") == 2);
//    assert(getResult(5, "YYYYYYYYYYYYYYYYYYYYYYYYY") == 2);
    assert(getResult(5, "NNNNNNNNNNNNNNNNNNNNNNNNN") == 0);
    assert(getResult(6, "NYNNNYYNYNYNNYNYNNNNYNNNNYNNNYYNNNYN") == 3);
    assert(getResult(6, "NYNNNYYNYNYNNYNNNYNNNNYNNYNYNNYNYNNN") == 3);
    assert(getResult(6, "NYNYNNYNYNNYNYNYYNYNYNNNNNYNNNNYNNNN") == 3);
}

void input() {
    int n;
    cin >> n;
    char letter;
    Graph g(n);

    for (int i = 0; i < n * n; ++i) {
        cin >> letter;
        g.addNode(i, letter);
    }

    cout << 2 * g.findMaxPairs();
}

int main() {
//    cout << getResult(3, "YYYYYYYYY");
//    cout << getResult(3, "NYYYNYYYN");
//    test();
    input();
    return 0;
}
