#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include "emp.h"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 4) {
        cerr << "Error" << endl;
        return 1;
    }

    const string input_file = argv[1];
    const string report_file = argv[2];
    double hourly_rate = atof(argv[3]);

    ifstream infile(input_file.c_str(), ios::binary);
    if (!infile) {
        cerr << "Error opening file for reading: " << input_file << endl;
        return 1;
    }

    ofstream report(report_file.c_str());
    if (!report) {
        cerr << "Error opening report file for writing: " << report_file << endl;
        return 1;
    }

    report << "Report for file \"" << input_file << "\"\n";
    report << "Employee ID  Employee Name  Hours Worked  Salary" << endl;

    employee emp;
    while (infile.read(reinterpret_cast<char*>(&emp), sizeof(emp))) {
        double salary = emp.hours * hourly_rate;
        report << setw(8) << emp.num << setw(18) << emp.name << setw(13) << emp.hours << setw(12) << salary << endl;
    }

    infile.close();
    report.close();

    return 0;
}
