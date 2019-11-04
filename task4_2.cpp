//
// Created by Анатолий Бурцев on 2019-11-03.
//

#include <iostream>

int find_pivot_index(const int *a, int aLen) {
    int a1Index = 0;
    int a2Index = aLen / 2;
    int a3Index = aLen - 1;
    int a1 = a[a1Index];
    int a2 = a[a2Index];
    int a3 = a[a3Index];

    if ((a1 >= a2 && a2 >= a3) || (a3 >= a2 && a2 >= a1)) {
        return a2Index;
    }

    if ((a2 >= a1 && a1 >= a3) || (a3 >= a1 && a1 >= a2)) {
        return a1Index;
    }

    if ((a1 >= a3 && a3 >= a2) || (a2 >= a3 && a3 >= a1)) {
        return a3Index;
    }
}

void swap(int *a, int src, int dst) {
    int tmp = a[src];
    a[src] = a[dst];
    a[dst] = tmp;
}

int Partition(int *a, int aLen) {
    if (aLen == 1) {
        return 0;
    }
    int pivotIndex = find_pivot_index(a, aLen);
    int pivot = a[pivotIndex];
    swap(a, pivotIndex, aLen - 1);
    int i = aLen - 2; // left border of numbers less than pivot
    int j = aLen - 2; // left border of numbers greater than pivot
    for (; i >= 0; i--) {
        if (a[i] > pivot) {
            swap(a, i, j--);
        }
    }
    swap(a, ++j, aLen - 1);
    return j;
}

int find_k_statistic(int *a, int aLen, int k) {
    while (true) {
        int pos = Partition(a, aLen);
        if (pos == k) {
            return a[pos];
        }
        if (aLen == 2) {
            return a[k];
        }
        if (pos < k) {
            a += (pos + 1);
            aLen -= (pos + 1);
            k -= (pos + 1);
        } else {
            a = a;
            aLen = (pos + 1);
            k = k;
        }
    }
}

int main() {
    int n;
    int k;
    std::cin >> n;
    std::cin >> k;
    auto array = new int[n];
    for (int i = 0; i < n; ++i) {
        std::cin >> array[i];
    }
    std::cout << find_k_statistic(array, n, k) << std::endl;

}
