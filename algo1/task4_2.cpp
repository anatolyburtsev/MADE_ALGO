//
// Created by Анатолий Бурцев on 2019-11-03.
// Даны неотрицательные целые числа n,k и массив целых чисел из [0..10^9] размера n.
// Требуется найти k-ю порядковую статистику. т.е. напечатать число, которое бы стояло на позиции
// с индексом k (0..n-1) в отсортированном массиве. Напишите нерекурсивный алгоритм.
//Требования к дополнительной памяти: O(n). Требуемое среднее время работы: O(n).
//Функцию Partition следует реализовывать методом прохода двумя итераторами в одном направлении.
// Описание для случая прохода от начала массива к концу:
//    • Выбирается опорный элемент. Опорный элемент меняется с последним элементом массива.
//    • Во время работы Partition в начале массива содержатся элементы, не бОльшие опорного.
//    Затем располагаются элементы, строго бОльшие опорного. В конце массива лежат нерассмотренные элементы.
//    Последним элементом лежит опорный.
//    • Итератор (индекс) i указывает на начало группы элементов, строго бОльших опорного.
//    • Итератор j больше i, итератор j указывает на первый нерассмотренный элемент.
//    • Шаг алгоритма. Рассматривается элемент, на который указывает j. Если он больше опорного, то сдвигаем j.
//Если он не больше опорного, то меняем a[i] и a[j] местами, сдвигаем i и сдвигаем j.
//    • В конце работы алгоритма меняем опорный и элемент, на который указывает итератор i.
// 4_2. Реализуйте стратегию выбора опорного элемента “медиана трёх”.
// Функцию Partition реализуйте методом прохода двумя итераторами от конца массива к началу.

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
    delete[] array;
}
