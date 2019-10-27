#include <iostream>
#include <cstring>
// Copyright [2019] <Burtsev Anatolii>

// Домашняя работа #1
// Задача 1.2 Дек с динамическим буфером
// Реализовать дек с динамическим зацикленным буфером.
// Обрабатывать команды push * и pop *.


class Deque {

public:
    void push_front(int new_element);

    void push_back(int new_element);

    int pop_front();

    int pop_back();

    Deque() {
        buffer_size = initial_size;
        buffer = new int[buffer_size];
        number_of_elements = 0;
        head_position = 0;
        tail_position = 0;
    }

    ~Deque() { delete[] buffer; }

    // function for debug purpose. Print some useful information
    // about inner state
    void print_buffer(std::string *message) {
        std::cout << "head: " << head_position << " tail: " << tail_position;
        std::cout << " no of elem: " << number_of_elements << std::endl;
        std::cout << "buffer state at " << message << " '";
        for (int i = 0; i < buffer_size; ++i) {
            std::cout << buffer[i] << ", ";
        }
        std::cout << "'" << std::endl;
    }

private:
    // initial size of buffer
    const int initial_size = 1;
    // symbol to return on pop operation in case of empty deque
    const int no_more_elements = -1;
    // when number of elements reached max,
    // increase buffer size in grow_coefficient times
    const int grow_coefficient = 2;
    // store all elements
    int *buffer;
    // current max size of buffer
    int buffer_size;
    // real number of elements in buffer
    int number_of_elements;
    // position of deque's head in buffer
    int head_position;
    // position of deque's tail in buffer
    int tail_position;

    void grow();
};

void Deque::grow() {
    int new_buffer_size = buffer_size * grow_coefficient;
    int *new_buffer = new int[new_buffer_size];

    if (head_position < tail_position) {
        memcpy(new_buffer, buffer,
               number_of_elements * sizeof(buffer[head_position]));
    } else {
        memcpy(new_buffer, buffer + head_position,
               (number_of_elements - head_position)
               * sizeof(buffer[head_position]));
        memcpy(new_buffer + number_of_elements - head_position,
               buffer, (tail_position + 1)
                       * sizeof(buffer[head_position]));
    }
    head_position = 0;
    tail_position = number_of_elements - 1;
    delete[] buffer;
    buffer = new_buffer;
    buffer_size = new_buffer_size;
}

void Deque::push_front(int new_element) {
    if (number_of_elements == buffer_size) {
        grow();
    }
    int new_head_position;
    if (number_of_elements > 0) {
        new_head_position = head_position == 0 ? (buffer_size - 1)
                                               : (head_position - 1);
    } else {
        // add element in empty buffer
        new_head_position = head_position;
        tail_position = head_position;
    }
    buffer[new_head_position] = new_element;
    head_position = new_head_position;
    number_of_elements += 1;
}

void Deque::push_back(int new_element) {
    if (number_of_elements == buffer_size) {
        grow();
    }
    int new_tail_position;
    if (number_of_elements > 0) {
        new_tail_position = tail_position == (buffer_size - 1) ? 0
                                                               : tail_position + 1;
    } else {
        // add element in empty buffer
        new_tail_position = tail_position;
        head_position = tail_position;
    }
    buffer[new_tail_position] = new_element;
    tail_position = new_tail_position;
    number_of_elements += 1;
}

int Deque::pop_front() {
    if (number_of_elements == 0) {
        return no_more_elements;
    }
    int result = buffer[head_position];
    buffer[head_position] = 0;
    number_of_elements -= 1;

    if (head_position < buffer_size - 1) {
        head_position += 1;
    } else {
        head_position = 0;
    }
    return result;
}

int Deque::pop_back() {
    if (number_of_elements == 0) {
        return no_more_elements;
    }
    int result = buffer[tail_position];
    buffer[tail_position] = 0;
    number_of_elements -= 1;

    if (tail_position > 0) {
        tail_position -= 1;
    } else {
        tail_position = buffer_size - 1;
    }
    return result;
}


int main() {
    Deque dequeue;
    int n = 0;
    std::cin >> n;
    int operation = 0;
    int number = 0;
    bool mismatch_found = false;

    for (int i = 0; i < n; ++i) {
        if (mismatch_found) break;
        std::cin >> operation;
        std::cin >> number;

        switch (operation) {
            case (1) : {
                dequeue.push_front(number);
                break;
            }
            case (2) : {
                int value = dequeue.pop_front();
                if (value != number) {
                    mismatch_found = true;
                }
                break;
            }
            case (3) : {
                dequeue.push_back(number);
                break;
            }
            case (4) : {
                int value = dequeue.pop_back();
                if (value != number) {
                    mismatch_found = true;
                }
                break;
            }
        }
    }
    if (mismatch_found) {
        std::cout << "NO" << std::endl;
    } else {
        std::cout << "YES" << std::endl;
    }
    return 0;
}

