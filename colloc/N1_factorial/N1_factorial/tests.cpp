#include <gtest/gtest.h>
#include "N1_factorial.h"

TEST(FactorialTests, HandlesZero) {
    auto res = calculate_factorials(0);
    EXPECT_TRUE(res.empty());
}

TEST(FactorialTests, HandlesSmallNumbers) {
    auto res = calculate_factorials(5);
    std::vector<uint64_t> expected = { 1, 2, 6, 24, 120 };
    EXPECT_EQ(res, expected);
}

TEST(FactorialTests, ThrowsOnTooLargeInput) {
    EXPECT_THROW(calculate_factorials(25), std::invalid_argument);
}

TEST(FactorialTests, HandlesOverflow) {
    EXPECT_THROW(calculate_factorials(21), std::invalid_argument);
}
