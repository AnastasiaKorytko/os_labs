#include "N1_factorial.h"

static const size_t MAX_N = 20;

std::vector<uint64_t> calculate_factorials(std::size_t n) {
    if (n == 0)
        return {};

    if (n > MAX_N)
        throw std::invalid_argument("invalid n");

    std::vector<uint64_t> result;
    result.reserve(n);

    uint64_t current = 1;
    for (std::size_t i = 1; i <= n; ++i) {
        if (current > std::numeric_limits<uint64_t>::max() / i)
            throw std::overflow_error("factorial overflow");

        current *= i;
        result.push_back(current);
    }

    return result;
}

