#include <gtest/gtest.h>
#include <fstream>
#include <filesystem>
#include "functions.h"

namespace fs = std::filesystem;

class Lab1Test : public ::testing::Test {
protected:
    const std::string bin_f = "test.bin";
    const std::string rep_f = "test.txt";

    void SetUp() override {
        employee e{ 1, "Test", 40.0 };
        std::ofstream out(bin_f, std::ios::binary);
        out.write(reinterpret_cast<char*>(&e), sizeof(e));
    }

    void TearDown() override {
        fs::remove(bin_f);
        fs::remove(rep_f);
    }
};

TEST_F(Lab1Test, FileCheck) {
    EXPECT_TRUE(fs::exists(bin_f));
}