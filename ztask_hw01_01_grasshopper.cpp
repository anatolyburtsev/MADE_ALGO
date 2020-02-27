#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;
//
// Created by Anatolii Burtsev on 2020-02-26.
//

void print(vector<int> max_costs) {
    for (int i = 0; i < max_costs.size(); ++i) {
        cout << max_costs[i] << " ";
    }
    cout << endl;
}

vector<int> get_backward_path(const vector<int>& prev_positions) {
    vector<int> path;
    int cur_pos = prev_positions.size()-1;
    path.push_back(cur_pos+1);
    while (cur_pos != 0) {
        cur_pos = prev_positions[cur_pos];
        path.push_back(cur_pos+1);
    }
    reverse(path.begin(), path.end());
    return path;
}

string calc(int n, int k, const vector<int> &costs) {
    vector<long> max_costs(n);
    fill(max_costs.begin(), max_costs.begin() + n, INT_MIN);
    max_costs[0] = 0;

    vector<int> jump_counts(n);
    jump_counts[0] = 0;

    vector<int> prev_positions(n);
    prev_positions[0] = 0;

    for (int i = 0; i < n; ++i) {
        for (int j = 1; j <= k; ++j) {
            if (i + j >= n) continue;
            long cost_to_get_there = max_costs[i] + costs[i + j];
            if (cost_to_get_there > max_costs[i + j]) {
                max_costs[i + j] = cost_to_get_there;
                jump_counts[i + j] = jump_counts[i] + 1;
                prev_positions[i + j] = i;
            }
        }
    }

    string output;
    output += std::to_string(max_costs[n-1]);
    output += "\n";
    output += std::to_string(jump_counts[n-1]);
    output += "\n";

    vector<int> path = get_backward_path(prev_positions);
    for (std::vector<int>::iterator it=path.begin(); it!=path.end(); ++it) {
        output += std::to_string(*it);
        if (it+1 != path.end()){
            output += " ";
        }
    }
    output += "\n";
    return output;
}


int main() {
    ifstream inFile;
//    inFile.open("/Users/anatoliiburtsev/CLionProjects/MADE_ALGO/input.txt_hw0101");
    inFile.open("input.txt");
    if (!inFile) {
        cerr << "Unable to open input file";
        exit(1);   // call system to stop
    }
    int n, k;
    inFile >> n;
    inFile >> k;
    vector<int> costs(n);
    costs[0] = 0;
    int tmp;
    for (int i = 1; i < n - 1; ++i) {
        inFile >> tmp;
        costs[i] = tmp;
    }
    inFile.close();
    costs[n - 1] = 0;
    string response = calc(n, k, costs);

    ofstream outFile;
    outFile.open("output.txt");
    if (!outFile) {
        cerr << "Unable to open output file";
        exit(1);   // call system to stop
    }
    outFile << response;
    outFile.close();

}

