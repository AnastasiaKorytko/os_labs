#include <iostream>
#include <fstream>
#include <string>
#include "employee.h"


int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Error" << std::endl;
        return 1;
    }

    const std::string filename = argv[1];
    int count = atoi(argv[2]);

    std::ofstream outfile(filename.c_str(), std::ios::binary);
    if (!outfile) {
        std::cerr << "Error opening file " << filename << std::endl;
        return 1;
    }

    employee emp;
    for (int i = 0; i < count; i++) {
        std::cout << "Enter details for employee " << i + 1 << ":" << std::endl;
        std::cout << "ID: ";
        std::cin >> emp.num;
        std::cin.ignore();
        std::cout << "Name: ";
        std::cin.getline(emp.name, sizeof(emp.name));
        std::cout << "Hours worked: ";
        std::cin >> emp.hours;

        outfile.write(reinterpret_cast<char*>(&emp), sizeof(emp));
    }

    outfile.close();
    return 0;
}

