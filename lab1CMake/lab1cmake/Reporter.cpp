#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <vector>
#include <algorithm>
#include "employee.h"

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: Reporter <binary_file> <report_file> <hourly_rate>" << std::endl;
        return 1;
    }

    const std::string input_file = argv[1];
    const std::string report_file = argv[2];
    double hourly_rate = std::atof(argv[3]);

    std::ifstream infile(input_file.c_str(), std::ios::binary);
    if (!infile) {
        std::cerr << "Error opening binary file: " << input_file << std::endl;
        return 1;
    }

    std::vector<employee> employees;
    employee emp;
    while (infile.read(reinterpret_cast<char*>(&emp), sizeof(emp))) {
        employees.push_back(emp);
    }
    infile.close();

    std::sort(employees.begin(), employees.end());

    std::ofstream report(report_file.c_str());
    if (!report) {
        std::cerr << "Error opening report file: " << report_file << std::endl;
        return 1;
    }

    report << "Report for file \"" << input_file << "\"\n";
    report << std::setw(8) << "ID" << std::setw(12) << "Name" << std::setw(8) << "Hours" << std::setw(12) << "Salary" << std::endl;

    for (size_t i = 0; i < employees.size(); ++i) {
        report << std::setw(8) << employees[i].num << std::setw(12) << employees[i].name << std::setw(8) << employees[i].hours << std::setw(12) << employees[i].salary(hourly_rate) << std::endl;
    }

    report.close();
    return 0;
}
