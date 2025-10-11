#pragma once
#include <string>
#include "employee.h"

void makeBinFile(const std::string& binary_filename);
void makeReport(const std::string& report_filename);
void printEmployee(const employee& e, double rate);
