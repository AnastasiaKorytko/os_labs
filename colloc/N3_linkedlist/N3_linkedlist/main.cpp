#include "N3_linkedlist.h"
#include <iostream>
#include <vector>
#include <limits>

const std::size_t MIN_LIST_SIZE = 1;
const int EXIT_SUCCESS_CODE = 0;
const int EXIT_ERROR_CODE = 1;
constexpr const char* ERROR_INVALID_SIZE = "Error: invalid list size";
constexpr const char* ERROR_INVALID_INPUT = "Error: invalid input";
constexpr const char* ERROR_EXTRA_INPUT = "Error: more numbers than expected";

int main() {
    std::size_t n;
    std::cout << "Enter number of elements: ";

    if (!(std::cin >> n) || n < MIN_LIST_SIZE) {
        std::cerr << ERROR_INVALID_SIZE << std::endl;
        return EXIT_ERROR_CODE;
    }

    std::vector<int> values;
    values.reserve(n);

    std::cout << "Enter " << n << " elements: ";
    for (std::size_t i = 0; i < n; i++) {
        int value;
        if (!(std::cin >> value)) {
            std::cerr << ERROR_INVALID_INPUT << std::endl;
            return EXIT_ERROR_CODE;
        }
        values.push_back(value);
    }

    if (std::cin.peek() != '\n' && !std::cin.eof()) {
        std::cerr << ERROR_EXTRA_INPUT << std::endl;
        return EXIT_ERROR_CODE;
    }

    auto head = create_list(values);
    std::cout << "Original list: ";
    print_list(head);

    auto reversed = reverse_list(head);
    std::cout << "Reversed list: ";
    print_list(reversed);

    return EXIT_SUCCESS_CODE;
}
