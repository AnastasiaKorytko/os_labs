#include <iostream>
#include <windows.h>
#include <fstream>
#include <string>
#include "emp.h"

using namespace std;

void makeBinFile(string binary_filename) {
    ifstream infile(binary_filename.c_str(), ios::binary);
    if (!infile) {
        cerr << "Error opening binary file: " << binary_filename << endl;
    }

    cout << "Contents of the binary file:" << endl;
    employee emp;
    while (infile.read(reinterpret_cast<char*>(&emp), sizeof(emp))) {
        cout << emp.num << " " << emp.name << " " << emp.hours << endl;
    }
    infile.close();
}

void makeReport(string report_filename) {
    ifstream report(report_filename.c_str());
    if (!report) {
        cerr << "Error opening report file: " << report_filename << endl;
    }

    cout << "Report generated:" << endl;
    string line;
    while (getline(report, line)) {
        cout << line << endl;
    }

    report.close();
}

int main() {
    string binary_filename, report_filename;
    int count;
    double hourly_rate;

    cout << "Enter binary filename: ";
    cin >> binary_filename;
    cout << "Enter number of records: ";
    cin >> count;

    string creator_cmd = "Creator " + binary_filename + " " + to_string(count);
    system(creator_cmd.c_str());

    DWORD exitCode;
    HANDLE hCreator = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, GetProcessId(GetCurrentProcess()));
    WaitForSingleObject(hCreator, INFINITE);
    CloseHandle(hCreator);

    makeBinFile(binary_filename);

    cout << "Enter report filename: ";
    cin >> report_filename;
    cout << "Enter hourly rate: ";
    cin >> hourly_rate;

    string reporter_cmd = "Reporter " + binary_filename + " " + report_filename + " " + to_string(hourly_rate);
    system(reporter_cmd.c_str());

    HANDLE hReporter = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, GetProcessId(GetCurrentProcess()));
    WaitForSingleObject(hReporter, INFINITE);
    CloseHandle(hReporter);

    makeReport(report_filename);

    return 0;
}
