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
    double hourly_rate = std::stod(argv[3]);

    std::ifstream infile(input_file, std::ios::binary);
    if (!infile) {
        std::cerr << "Error opening binary file: " << input_file << std::endl;
        return 1;
    }

    std::vector<employee> employees;
    employee emp;
    while (infile.read(reinterpret_cast<char*>(&emp), sizeof(employee))) {
        employees.push_back(emp);
    }
    infile.close();

    std::sort(employees.begin(), employees.end());

    std::ofstream report(report_file);
    if (!report) {
        std::cerr << "Error opening report file: " << report_file << std::endl;
        return 1;
    }

    report << "Report for file \"" << input_file << "\"\n";
    report << std::setw(8) << "ID" << std::setw(12) << "Name"
        << std::setw(8) << "Hours" << std::setw(12) << "Salary" << std::endl;

    for (const auto& e : employees) {
        report << std::setw(8) << e.num
            << std::setw(12) << e.name
            << std::setw(8) << e.hours
            << std::setw(12) << e.salary(hourly_rate) << std::endl;
    }

    report.close();
    return 0;
}