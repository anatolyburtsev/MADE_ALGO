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

string find_balanced_brace_seq(const string& input) {
    int n = input.size();
    vector<vector<int>> dp(n);


    for (int j = n-1; j >= 0; ++j) {
        for (int i = j+1; i < n; ++i) {

        }
    }

    return input;
}


int main() {
    string input = "({]})";

    find_balanced_brace_seq(input);
    return 0;
}
