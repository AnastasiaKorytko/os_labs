#include <iostream>
#include <string>
#include <filesystem>
#include <cstdlib>
#include "employee.h"
#include "functions.h"

int main() {
    std::string binary_filename, report_filename;
    int count;
    double hourly_rate;

    std::cout << "Enter binary filename: ";
    std::cin >> binary_filename;
    std::cout << "Enter number of records: ";
    std::cin >> count;
    std::filesystem::path path = std::filesystem::current_path();
    std::string creator_cmd = "\"" + (path / "Creator.exe").string() + "\" " + binary_filename + " " + std::to_string(count);
    std::system(creator_cmd.c_str());
    makeBinFile(binary_filename);

    std::cout << "Enter report filename: ";
    std::cin >> report_filename;
    std::cout << "Enter hourly rate: ";
    std::cin >> hourly_rate;

    std::string reporter_cmd = "\"" + (path / "Reporter.exe").string() + "\" " + binary_filename + " " + report_filename + " " + std::to_string(hourly_rate);
    std::system(reporter_cmd.c_str());

    makeReport(report_filename);

    return 0;
}