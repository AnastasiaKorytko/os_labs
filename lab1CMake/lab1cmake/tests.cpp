#include <gtest/gtest.h>
#include <fstream>
#include <sstream>
#include <cstdio>
#include "employee.h"
#include "functions.h"

class Lab1Test : public ::testing::Test {
protected:
    const char* bin_filename;
    const char* report_filename;

    Lab1Test() : bin_filename("test_emp.bin"), report_filename("test_report.txt") {}

    void SetUp() {
        createTestBinFile();
        createTestReportFile();
    }

    void TearDown() {
        remove(bin_filename);
        remove(report_filename);
    }

    std::string captureMakeBinFileOutput() {
        std::ostringstream output;
        std::streambuf* oldCout = std::cout.rdbuf(output.rdbuf());
        makeBinFile(bin_filename);
        std::cout.rdbuf(oldCout);
        return output.str();
    }

    std::string captureMakeReportOutput() {
        std::ostringstream output;
        std::streambuf* oldCout = std::cout.rdbuf(output.rdbuf());
        makeReport(report_filename);
        std::cout.rdbuf(oldCout);
        return output.str();
    }

private:
    void createTestBinFile() {
        employee e;
        e.num = 1;
        strcpy(e.name, "John");
        e.hours = 40.0;

        std::ofstream out(bin_filename, std::ios::binary);
        out.write(reinterpret_cast<char*>(&e), sizeof(e));
        out.close();
    }

    void createTestReportFile() {
        std::ofstream report(report_filename);
        report << "Report header\nLine1\nLine2\n";
        report.close();
    }
};

TEST_F(Lab1Test, MakeBinFilePrintsCorrectly) {

    std::string output = captureMakeBinFileOutput();

    EXPECT_NE(output.find("John"), std::string::npos);
}

TEST_F(Lab1Test, MakeReportPrintsCorrectly) {
   
    std::string output = captureMakeReportOutput();

    EXPECT_NE(output.find("Line1"), std::string::npos);
}

