#include <gtest/gtest.h>
#include "marker_system.h"
#include <thread>
#include <algorithm>

TEST(MarkerArrayTest, InitializesWithZeros) {
    const std::size_t N = 5;
    MarkerArray a(N);
    auto copy = a.copy();

    EXPECT_EQ(copy.size(), N);
    EXPECT_TRUE(std::all_of(copy.begin(), copy.end(), [](int v) {
        return v == 0;
        }));
}

TEST(MarkerArrayTest, TryMarkAndPreventOverwrite) {
    MarkerArray a(3);
    int id1 = 1, id2 = 2;
    bool ok1 = a.try_mark(1, id1);
    bool ok2 = a.try_mark(1, id2);

    EXPECT_TRUE(ok1);
    EXPECT_FALSE(ok2);
    auto copy = a.copy();
    EXPECT_EQ(copy[1], id1);
}

TEST(MarkerArrayTest, ClearMarksOnlyOwn) {
    MarkerArray a(4);
    a.try_mark(0, 1);
    a.try_mark(1, 2);
    a.try_mark(2, 1);
    a.clear_marks(1);

    auto copy = a.copy();
    EXPECT_EQ(copy[0], 0);
    EXPECT_EQ(copy[1], 2);
    EXPECT_EQ(copy[2], 0);
}

TEST(MarkerArrayTest, CountMarkedBy) {
    MarkerArray a(6);
    a.try_mark(0, 3);
    a.try_mark(1, 3);
    a.try_mark(4, 2);
    auto c3 = a.count_marked_by(3);
    auto c2 = a.count_marked_by(2);

    EXPECT_EQ(c3, 2);
    EXPECT_EQ(c2, 1);
}

TEST(MarkerArrayTest, ThreadSafetyStress) {
    MarkerArray a(50);
    const int marker_id = 7;
    std::vector<std::thread> thr;
    for (int i = 0; i < 8; ++i) {
        thr.emplace_back([&a, marker_id]() {
            for (int k = 0; k < 1000; k++) {
                std::size_t idx = static_cast<std::size_t>(k % a.size());
                a.try_mark(idx, marker_id);
            }
            });
    }
    for (auto& t : thr) t.join();

    auto copy = a.copy();
    EXPECT_EQ(copy.size(), 50u);
    for (int v : copy)
        EXPECT_TRUE(v == 0 || v == marker_id);
}
