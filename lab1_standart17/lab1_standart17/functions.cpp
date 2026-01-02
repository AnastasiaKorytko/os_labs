#include "functions.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string> 

void printEmployee(const employee& e, double rate) {
    std::cout << std::left << std::setw(8) << e.num
        << std::setw(12) << e.name
        << std::setw(8) << e.hours
        << std::setw(12) << e.salary(rate)
        << std::endl;
}

void makeBinFile(std::string_view binary_filename) {
    std::ifstream infile(binary_filename.data(), std::ios::binary);
    if (!infile) {
        std::cerr << "Error opening binary file: " << binary_filename << std::endl;
        return;
    }

    std::cout << "\n    Contents of binary file:    \n";
    employee emp;
    while (infile.read(reinterpret_cast<char*>(&emp), sizeof(employee))) {
        printEmployee(emp, 0.0);
    }
}

void makeReport(std::string_view report_filename) {
    std::ifstream report(report_filename.data());
    if (!report) {
        std::cerr << "Error opening report file: " << report_filename << std::endl;
        return;
    }

    std::cout << "\n    Report generated:    \n";
    std::string line;
    while (std::getline(report, line)) {
        std::cout << line << std::endl;
    }
}