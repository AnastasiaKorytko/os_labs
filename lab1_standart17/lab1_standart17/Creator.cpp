#include <iostream>
#include <fstream>
#include <string>
#include "employee.h"

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: Creator <binary_file> <number_of_records>" << std::endl;
        return 1;
    }

    const std::string filename = argv[1];
    int count = std::stoi(argv[2]); 
    
    std::ofstream outfile(filename, std::ios::binary);
    if (!outfile) {
        std::cerr << "Error opening file " << filename << std::endl;
        return 1;
    }

    employee emp;
    for (int i = 0; i < count; ++i) {
        std::cout << "Enter ID, name and hours worked " << i + 1 << ":" << std::endl;
        std::cin >> emp;
        outfile.write(reinterpret_cast<const char*>(&emp), sizeof(employee));
    }

    outfile.close();
    return 0;
}