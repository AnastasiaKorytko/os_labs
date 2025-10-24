#include <gtest/gtest.h>
#include "N2_duplicates.h"
#include <vector>

TEST(RemoveDuplicatesTests, HandlesEmptyVector) {
    std::vector<int> input;
    auto output = remove_duplicates(input);
    EXPECT_TRUE(output.empty());
}

TEST(RemoveDuplicatesTests, HandlesNoDuplicates) {
    std::vector<int> input = { 1, 2, 3, 4 };
    auto output = remove_duplicates(input);
    EXPECT_EQ(output, input);
}

TEST(RemoveDuplicatesTests, RemovesDuplicates) {
    std::vector<int> input = { 1, 2, 2, 3, 1, 4 };
    std::vector<int> expected = { 1, 2, 3, 4 };
    auto output = remove_duplicates(input);
    EXPECT_EQ(output, expected);
}

TEST(RemoveDuplicatesTests, PreservesOrder) {
    std::vector<int> input = { 5, 3, 5, 2, 3 };
    std::vector<int> expected = { 5, 3, 2 };
    auto output = remove_duplicates(input);
    EXPECT_EQ(output, expected);
}
