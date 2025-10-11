#include "functions.h"
#include <iostream>
#include <fstream>
#include <iomanip>

void printEmployee(const employee& e, double rate) {
    std::cout << std::setw(8) << e.num
        << std::setw(12) << e.name
        << std::setw(8) << e.hours
        << std::setw(12) << e.salary(rate)
        << std::endl;
}

void makeBinFile(const std::string& binary_filename) {
    std::ifstream infile(binary_filename.c_str(), std::ios::binary);
    if (!infile) {
        std::cerr << "Error opening binary file: " << binary_filename << std::endl;
        return;
    }

    std::cout << "Contents of the binary file:" << std::endl;
    employee emp;
    while (infile.read(reinterpret_cast<char*>(&emp), sizeof(emp))) {
        printEmployee(emp, 0.0); 
    }
    infile.close();
}

void makeReport(const std::string& report_filename) {
    std::ifstream report(report_filename.c_str());
    if (!report) {
        std::cerr << "Error opening report file: " << report_filename << std::endl;
        return;
    }

    std::cout << "Report generated:" << std::endl;
    std::string line;
    while (std::getline(report, line)) {
        std::cout << line << std::endl;
    }
    report.close();
}
