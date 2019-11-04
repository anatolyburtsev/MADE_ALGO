#include <iostream>
#include <cstring>

//
// Created by Анатолий Бурцев on 2019-10-29.
//

void MergeTwoSortedArrays(const int *a, int aLen, const int *b, int bLen, int *c) {
    int ia = 0;
    int ib = 0;
    int ic = 0;
    while (ia < aLen && ib < bLen) {
        if (a[ia] > b[ib]) {
            c[ic] = b[ib];
            ib++;
            ic++;
        } else {
            c[ic] = a[ia];
            ia++;
            ic++;
        }
    }
    if (ia == aLen) {
        for (int i = ib; i < bLen; ++i) {
            c[ic++] = b[i];
        }
    } else {
        for (int i = ia; i < aLen; ++i) {
            c[ic++] = a[i];
        }
    }
}

void MergeSort(int *a, int aLen) {
    if (aLen <= 1) {
        return;
    }

    int firstLen = aLen / 2;
    int secondLen = aLen - firstLen;
    MergeSort(a, firstLen);
    MergeSort(a + firstLen, secondLen);
    int *c = new int[aLen];
    MergeTwoSortedArrays(a, firstLen, a + firstLen, secondLen, c);
    memcpy(a, c, sizeof(int) * aLen);
    delete[] c;
}

void print(const int *a, int aLen) {
    for (int i = 0; i < aLen; ++i) {
        std::cout << a[i] << " ";
    }
    std::cout << std::endl;
}

int *read_new_batch(int k) {
    auto data = new int[k];
    for (int i = 0; i < k; ++i) {
        std::cin >> data[i];
    }
    return data;
}

int *do_it(int n, int k) {
    auto unsorted_data = read_new_batch(k);
    MergeSort(unsorted_data, k);
    auto sorted_data = unsorted_data;

    int i = 1;
    for (; i < n / k; ++i) {
        auto new_batch_of_data = read_new_batch(k);
        MergeSort(new_batch_of_data, k);
        auto updated_sorted_data = new int[2 * k];
        MergeTwoSortedArrays(sorted_data, k, new_batch_of_data, k, updated_sorted_data);
        delete[] sorted_data;
        delete[] new_batch_of_data;
        sorted_data = updated_sorted_data;
    }

    int tail_size = n - i * k;
    auto new_batch_of_data = read_new_batch(tail_size);
    MergeSort(new_batch_of_data, tail_size);
    auto updated_sorted_data = new int[k + tail_size];
    MergeTwoSortedArrays(sorted_data, k, new_batch_of_data, tail_size, updated_sorted_data);
    delete[] new_batch_of_data;
    delete sorted_data;

    return updated_sorted_data;
}


int main() {
    int n;
    int k;
    std::cin >> n;
    std::cin >> k;
    int *result = do_it(n, k);
    print(result, k);
}