#include "pch.h"
#include "CppUnitTest.h"
#include "C:\os\lab1\StaticLib1\StaticLib1\emp.h"
#include <fstream>
#include "C:\os\lab1\StaticLib1\StaticLib1\Main.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

extern void makeBinFile(string binary_filename);
extern void makeReport(string report_filename);

namespace UnitTest1
{
	TEST_CLASS(UnitTest1)
	{
    public:

        TEST_METHOD(Test_makeBinFile_prints_correctly)
        {
            string filename = "test_bin_file.bin";
            employee emps[2] = {
                {1, "Anna", 12.5},
                {2, "John", 35.0}
            };

            ofstream out(filename, ios::binary);
            out.write(reinterpret_cast<const char*>(&emps[0]), sizeof(employee));
            out.write(reinterpret_cast<const char*>(&emps[1]), sizeof(employee));
            out.close();

            streambuf* original_cout = cout.rdbuf();
            ostringstream capture;
            cout.rdbuf(capture.rdbuf());

            makeBinFile(filename);

            cout.rdbuf(original_cout);

            string output = capture.str();

            Assert::IsTrue(output.find("1 Anna 12.5") != string::npos);
            Assert::IsTrue(output.find("2 John 35") != string::npos);

            remove(filename.c_str());
        }

        TEST_METHOD(Test_makeReport_prints_correctly)
        {
            string filename = "test_report.txt";
            ofstream out(filename);
            out << "Report for file \"data.bin\"\n";
            out << "Employee ID  Employee Name  Hours Worked  Salary\n";
            out << "1           Anna            12.5           625\n";
            out.close();

            streambuf* original_cout = cout.rdbuf();
            ostringstream capture;
            cout.rdbuf(capture.rdbuf());

            makeReport(filename);

            cout.rdbuf(original_cout);

            string output = capture.str();

            Assert::IsTrue(output.find("Report for file") != string::npos);
            Assert::IsTrue(output.find("Anna") != string::npos);
            Assert::IsTrue(output.find("625") != string::npos);

            remove(filename.c_str());
        }
	};
}
