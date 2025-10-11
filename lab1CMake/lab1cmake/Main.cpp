#include <iostream>
#include <windows.h>
#include <string>
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

    std::string creator_cmd = "C:\\Users\\Anastasia\\source\\repos\\AnastasiaKorytko\\os_labs\\lab1CMake\\out\\build\\x64-debug\\lab1CMake.\\Creator " + binary_filename + " " + std::to_string(count);
    system(creator_cmd.c_str());

    makeBinFile(binary_filename);

    std::cout << "Enter report filename: ";
    std::cin >> report_filename;
    std::cout << "Enter hourly rate: ";
    std::cin >> hourly_rate;

    std::string reporter_cmd = "C:\\Users\\Anastasia\\source\\repos\\AnastasiaKorytko\\os_labs\\lab1CMake\\out\\build\\x64-debug\\lab1CMake.\\Reporter " + binary_filename + " " + report_filename + " " + std::to_string(hourly_rate);
    system(reporter_cmd.c_str());

    makeReport(report_filename);

    return 0;
}


