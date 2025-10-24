#include <iostream>
#include "N1_factorial.h"

int main() {
    size_t n;
    std::cout << "Enter n: ";
    std::cin >> n;

    try {
        auto factorials = calculate_factorials(n);
        for (size_t i = 0; i < factorials.size(); ++i) {
            std::cout << (i + 1) << "! = " << factorials[i] << std::endl;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}

