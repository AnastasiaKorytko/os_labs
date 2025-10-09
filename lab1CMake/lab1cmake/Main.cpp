#include <iostream>
#include <windows.h>
#include <fstream>
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

    std::string creator_cmd = "Creator " + binary_filename + " " + std::to_string(count);
    system(creator_cmd.c_str());

    DWORD exitCode;
    HANDLE hCreator = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, GetProcessId(GetCurrentProcess()));
    WaitForSingleObject(hCreator, INFINITE);
    CloseHandle(hCreator);

    makeBinFile(binary_filename);

    std::cout << "Enter report filename: ";
    std::cin >> report_filename;
    std::cout << "Enter hourly rate: ";
    std::cin >> hourly_rate;

    std::string reporter_cmd = "Reporter " + binary_filename + " " + report_filename + " " + std::to_string(hourly_rate);
    system(reporter_cmd.c_str());

    HANDLE hReporter = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, GetProcessId(GetCurrentProcess()));
    WaitForSingleObject(hReporter, INFINITE);
    CloseHandle(hReporter);

    makeReport(report_filename);
    return 0;
}
