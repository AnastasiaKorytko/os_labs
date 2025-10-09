#include "employee.h"
#include <fstream>
#include <iostream>
#include <string>

void makeBinFile(std::string binary_filename) {
    std::ifstream infile(binary_filename.c_str(), std::ios::binary);
    if (!infile) {
        std::cerr << "Error opening binary file: " << binary_filename << std::endl;
        return; 
    }

    std::cout << "Contents of the binary file:" << std::endl;
    employee emp;
    while (infile.read(reinterpret_cast<char*>(&emp), sizeof(emp))) {
        std::cout << emp.num << " " << emp.name << " " << emp.hours << std::endl;
    }
    infile.close();
}

void makeReport(std::string report_filename) {
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