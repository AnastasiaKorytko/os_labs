#include <gtest/gtest.h>
#include "N3_linkedlist.h"

TEST(LinkedListTests, HandlesEmptyList) {
    auto list = create_list({});
    EXPECT_EQ(list, nullptr);
}

TEST(LinkedListTests, CreatesListCorrectly) {
    auto list = create_list({ 1, 2, 3 });
    auto vec = list_to_vector(list);
    EXPECT_EQ(vec, (std::vector<int>{1, 2, 3}));
}

TEST(LinkedListTests, ReversesListCorrectly) {
    auto list = create_list({ 1, 2, 3, 4 });
    auto reversed = reverse_list(list);
    auto vec = list_to_vector(reversed);
    EXPECT_EQ(vec, (std::vector<int>{4, 3, 2, 1}));
}

TEST(LinkedListTests, HandlesSingleElementList) {
    auto list = create_list({ 42 });
    auto reversed = reverse_list(list);
    auto vec = list_to_vector(reversed);
    EXPECT_EQ(vec, (std::vector<int>{42}));
}

TEST(LinkedListTests, HandlesNullptr) {
    std::shared_ptr<Node> head = nullptr;
    EXPECT_EQ(reverse_list(head), nullptr);
}