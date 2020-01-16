//
// Created by Анатолий Бурцев on 2019-12-24.
// Найдите все вхождения шаблона в строку. Длина шаблона – p, длина строки – n. Время O(n + p), доп. память – O(p).
// p <= 30000, n <= 300000.
// Использовать один из методов:
// - С помощью префикс-функции;
//

#include <string>
#include <iostream>
#include <vector>

class ConcatenatedString {
public:

    ConcatenatedString(std::string &prefix, std::string &suffix) : prefix(prefix), suffix(suffix) {}

    char at(int index) {
        if (index < prefix.length()) {
            return prefix.at(index);
        } else if (index > prefix.length()) {
            return suffix.at(index - prefix.length() - 1);
        } else {
            return '$';
        }
    }

private:
    std::string& prefix;
    std::string& suffix;
};

class SubstringFinder {
public:
    SubstringFinder(std::string &text, std::string &pattern) :
            pattern(pattern),
            p(pattern.size()),
            concatenatedString(pattern, text),
            n(text.size()) {}

    std::vector<int> find_substrings();

private:
    std::vector<int> calculate_prefix_function(std::string &S);

    ConcatenatedString concatenatedString;
    std::string pattern;
    int p;
    int n;
};

std::vector<int> SubstringFinder::calculate_prefix_function(std::string &S) {
    std::vector<int> result(S.size());
    result.at(0) = 0;
    for (int i = 1; i < S.size(); ++i) {
        int left_index = i;
        int right_index = result.at(i - 1);
        while (right_index > 0 && S.at(left_index) != S.at(right_index)) {
            right_index = result.at(right_index - 1);
        }
        if (S.at(left_index) == S.at(right_index)) {
            result[i] = right_index + 1;
        } else {
            result[i] = 0;
        }
    }
    return result;
}

std::vector<int> SubstringFinder::find_substrings() {
    std::vector<int> result;
    std::vector<int> pattern_pi = calculate_prefix_function(pattern);
    // imagine string: pattern$longlongtext
    int prev_pi_value = 0;
    for (int i = p + 1; i < n + p + 1; ++i) {
        int left_index = i;
        int right_index = prev_pi_value;
        while (right_index > 0 && concatenatedString.at(left_index) != concatenatedString.at(right_index)) {
            right_index = pattern_pi.at(right_index - 1);
        }
        if (concatenatedString.at(left_index) == concatenatedString.at(right_index)) {
            prev_pi_value = right_index + 1;
            if (right_index + 1 == p) {
                result.push_back(left_index - 2 * p);
            }
        } else {
            prev_pi_value = 0;
        }

    }

    return result;
}


int main() {
    std::string pattern;
    std::string text;
    std::cin >> pattern;
    std::cin >> text;
    SubstringFinder fndr(text, pattern);

    auto result = fndr.find_substrings();
    for (int i : result) {
        std::cout << i << " ";
    }
    return 0;
}
