#include "N2_duplicates.h"
#include <vector>
#include <algorithm>

std::vector<int> remove_duplicates(const std::vector<int>& input) {
    std::vector<int> result;
    for (int el : input) {
        if (std::find(result.begin(), result.end(), el) == result.end()) {
            result.push_back(el);
        }
    }
    return result;
}

