#include <iostream>
#include <vector>
#include "N2_duplicates.h"
#include <limits>

static const std::size_t MAX_INPUT = 1000;

int main() {
    std::size_t n;
    std::cout << "Enter the number of elements (max " << MAX_INPUT << "): ";

    if (!(std::cin >> n) || n == 0 || n > MAX_INPUT) {
        std::cerr << "Error: invalid number of elements" << std::endl;
        return 1;
    }

    std::vector<int> numbers;
    numbers.reserve(n);

    std::cout << "Enter " << n << " elements ";
    int value;
    for (std::size_t i = 0; i < n; i++) {
        if (!(std::cin >> value)) {
            std::cerr << "Error: invalid input" << std::endl;
            return 1;
        }
        numbers.push_back(value);
    }

    if (std::cin >> value) {
        std::cerr << "Error: more elements than expected" << std::endl;
        return 1;
    }

    try {
        auto unique = remove_duplicates(numbers);

        std::cout << "Unique elements: ";
        for (int el : unique) {
            std::cout << el << " ";
        }
        std::cout << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Unexpected error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
