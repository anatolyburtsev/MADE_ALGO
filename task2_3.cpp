//
// Created by Анатолий Бурцев on 2019-10-19.
//

// Домашняя работа #1
// Задача 2.3 Тупики
//На вокзале есть некоторое количество тупиков, куда
//прибывают электрички. Этот вокзал является их конечной станцией.
//Дано расписание движения электричек, в котором для каждой электрички
//указано время ее прибытия, а также время отправления в следующий
//рейс. Электрички в расписании упорядочены по времени прибытия.
//Когда электричка прибывает, ее ставят в свободный тупик с
//минимальным номером. При этом если электричка из какого-то тупика
//отправилась в момент времени X, то электричку, которая прибывает
//в момент времени X, в этот тупик ставить нельзя, а электричку,
//прибывающую в момент X+1 — можно. В данный момент на вокзале
//достаточное количество тупиков для работы по расписанию. Напишите
//программу, которая по данному расписанию определяет, какое
//минимальное количество тупиков требуется для работы вокзала.

#include <iostream>
#include <vector>

// #define DEBUG

class MinHeap {
public:
    void push(int element);

    int pop();

    int get_min();

    void print_buffer(const std::string &message) {
        std::cout << message << ": ";
        for (int i : arr) {
            std::cout << i << ", ";
        }
        std::cout << std::endl;
    }

    int size() {
        return arr.size();
    }

    bool empty() {
        return arr.empty();
    }

private:
    std::vector<int> arr = {};

    void sift_up(int position);

    void sift_down(int position);

    static int get_left_child_index(int current_index) {
        return 2 * current_index + 1;
    }

    static int get_right_child_index(int current_index) {
        return 2 * current_index + 2;
    }

    static int get_parent_index(int current_index) {
        return (current_index - 1) / 2;
    }
};

void MinHeap::sift_up(int position) {
    if (arr.empty()) { return; }
    int current_position = position;
    while (current_position > 0) {
        int child_index = current_position;
        int parent_index = get_parent_index(child_index);
        int child = arr.at(child_index);
        int parent = arr.at(parent_index);
        if (child < parent) {
            arr.at(child_index) = parent;
            arr.at(parent_index) = child;
            current_position = child_index;
        } else {
            return;
        }
    }
}

void MinHeap::sift_down(int position) {
    if (arr.empty()) { return; }
    int parent_index = position;
    int parent = arr.at(parent_index);
    while (parent_index <= arr.size() - 1) {
        int left_child_index = get_left_child_index(parent_index);
        int right_child_index = get_right_child_index(parent_index);
        if (left_child_index > arr.size() - 1) {
            // if no left child => no right child too. reached leaf
            return;
        } else if (right_child_index > arr.size() - 1) {
            int left_child = arr.at(left_child_index);
            if (left_child < parent) {
                arr.at(left_child_index) = parent;
                arr.at(parent_index) = left_child;
            }
            return;
        } else {
            int left_child = arr.at(left_child_index);
            int right_child = arr.at(right_child_index);
            int min_child;
            int min_child_index;
            if (left_child < right_child) {
                min_child = left_child;
                min_child_index = left_child_index;
            } else {
                min_child = right_child;
                min_child_index = right_child_index;
            }
            if (min_child < parent) {
                arr.at(min_child_index) = parent;
                arr.at(parent_index) = min_child;
                parent_index = min_child_index;
            } else {
                // both children are larger than parent
                return;
            }
        }
    }
}

void MinHeap::push(int element) {
    arr.push_back(element);
#ifdef DEBUG
    print_buffer("push before sift up");
#endif
    sift_up(arr.size() - 1);
#ifdef DEBUG
    print_buffer("push after sift up");
#endif
}

int MinHeap::get_min() {
    int min = arr.empty() ? -1 : arr.at(0);
    return min;
}

int MinHeap::pop() {
    int min_value = arr.at(0);
    arr.at(0) = arr.at(arr.size() - 1);
    arr.pop_back();
#ifdef DEBUG
    print_buffer("pop before sift down");
#endif
    sift_down(0);
#ifdef DEBUG
    print_buffer("pop after sift down");
#endif
    return min_value;
}

int main() {
    auto *heap = new MinHeap();
    int max_amount_of_trains = 0;
    int arrival_time;
    int departure_time;
    int n;
    std::cin >> n;
    for (int i = 0; i < n; ++i) {
        std::cin >> arrival_time;
        std::cin >> departure_time;
        while (heap->get_min() < arrival_time && !heap->empty()) {
            heap->pop();
        }
        heap->push(departure_time);
        max_amount_of_trains = heap->size() > max_amount_of_trains ?
                               heap->size() : max_amount_of_trains;
    }
    std::cout << max_amount_of_trains << std::endl;
    return 0;
}
