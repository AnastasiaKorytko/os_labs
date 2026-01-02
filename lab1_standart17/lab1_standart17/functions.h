#pragma once
#include <string_view>
#include "employee.h"

void makeBinFile(std::string_view binary_filename);
void makeReport(std::string_view report_filename);
void printEmployee(const employee& e, double rate);