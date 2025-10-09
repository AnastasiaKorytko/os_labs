#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include "employee.h"

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Error" << std::endl;
        return 1;
    }

    const std::string input_file = argv[1];
    const std::string report_file = argv[2];
    double hourly_rate = atof(argv[3]);

    std::ifstream infile(input_file.c_str(), std::ios::binary);
    if (!infile) {
        std::cerr << "Error opening file for reading: " << input_file << std::endl;
        return 1;
    }

    std::ofstream report(report_file.c_str());
    if (!report) {
        std::cerr << "Error opening report file for writing: " << report_file << std::endl;
        return 1;
    }

    report << "Report for file \"" << input_file << "\"\n";
    report << "Employee ID  Employee Name  Hours Worked  Salary" << std::endl;

    employee emp;
    while (infile.read(reinterpret_cast<char*>(&emp), sizeof(emp))) {
        double salary = emp.hours * hourly_rate;
        report << std::setw(8) << emp.num << std::setw(18) << emp.name << std::setw(13) << emp.hours << std::setw(12) << salary << std::endl;
    }

    infile.close();
    report.close();

    return 0;
}
