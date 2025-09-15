#include <iostream>
#include <fstream>
#include <string>
#include "emp.h"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "Error" << endl;
        return 1;
    }

    const string filename = argv[1];
    int count = atoi(argv[2]);

    ofstream outfile(filename.c_str(), ios::binary);
    if (!outfile) {
        cerr << "Error opening file " << filename << endl;
        return 1;
    }

    employee emp;
    for (int i = 0; i < count; i++) {
        cout << "Enter details for employee " << i + 1 << ":" << endl;
        cout << "ID: ";
        cin >> emp.num;
        cin.ignore();
        cout << "Name: ";
        cin.getline(emp.name, sizeof(emp.name));
        cout << "Hours worked: ";
        cin >> emp.hours;

        outfile.write(reinterpret_cast<char*>(&emp), sizeof(emp));
    }

    outfile.close();
    return 0;
}
