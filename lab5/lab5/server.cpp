#include "common.h"
#include <windows.h>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>

class PipeHandle {
public:
    explicit PipeHandle(HANDLE handle) : handle_(handle) {}
    ~PipeHandle() { if (handle_ != INVALID_HANDLE_VALUE) CloseHandle(handle_); }
    HANDLE get() const { return handle_; }
private:
    HANDLE handle_{ INVALID_HANDLE_VALUE };
};

bool WriteToPipe(HANDLE pipe, const void* data, size_t size) {
    DWORD written{};
    return WriteFile(pipe, data, static_cast<DWORD>(size), &written, nullptr) && written == size;
}

bool ReadFromPipe(HANDLE pipe, void* data, size_t size) {
    DWORD read{};
    return ReadFile(pipe, data, static_cast<DWORD>(size), &read, nullptr) && read == size;
}

void PrintEmployee(const Employee& e) {
    std::cout << "ID=" << e.id
        << " Name='" << std::string(e.name.data(), e.name.size()) << "'"
        << " Hours=" << e.hours << std::endl;
}

bool SaveEmployees(const std::string& filename, const std::vector<Employee>& employees) {
    std::ofstream file(filename, std::ios::binary | std::ios::trunc);
    if (!file) return false;
    for (const auto& e : employees) file.write(reinterpret_cast<const char*>(&e), sizeof(Employee));
    return true;
}

bool LoadEmployees(const std::string& filename, std::vector<Employee>& employees) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) return false;
    Employee e{};
    while (file.read(reinterpret_cast<char*>(&e), sizeof(Employee))) employees.push_back(e);
    return true;
}

Employee* FindEmployee(std::vector<Employee>& employees, int id) {
    for (auto& e : employees) if (e.id == id) return &e;
    return nullptr;
}

int main() {
    std::cout << "Windows IPC Server" << std::endl;

    std::string filename;
    std::cout << "Enter filename for employees: ";
    std::getline(std::cin, filename);

    int n{};
    std::cout << "Enter number of employees: ";
    std::cin >> n; std::cin.ignore();

    std::vector<Employee> employees;
    employees.reserve(n);

    for (int i = 0; i < n; ++i) {
        Employee e{};
        std::cout << "Employee ID: ";
        std::cin >> e.id; std::cin.ignore();

        std::string name;
        std::cout << "Name: "; std::getline(std::cin, name);

        std::fill(e.name.begin(), e.name.end(), 0);
        
        size_t max_copy = e.name.size() - 1;
        size_t copy_len = (name.size() < max_copy) ? name.size() : max_copy;
        for (size_t j = 0; j < copy_len; ++j) e.name[j] = name[j];

        std::cout << "Hours: "; std::cin >> e.hours; std::cin.ignore();
        employees.push_back(e);
    }

    if (!SaveEmployees(filename, employees)) {
        std::cerr << "Failed to save employees" << std::endl;
        return 1;
    }

    PipeHandle serverPipe(CreateNamedPipeW(
        kServerPipeName,
        PIPE_ACCESS_DUPLEX,
        PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
        PIPE_UNLIMITED_INSTANCES,
        512, 512, 0, nullptr));

    if (serverPipe.get() == INVALID_HANDLE_VALUE) {
        std::cerr << "Failed to create server pipe " << std::endl;
        return 1;
    }

    std::cout << "Server listening on pipe..." << std::endl;

    while (true) {
        std::cout << "Waiting for client..." << std::endl;
        if (!ConnectNamedPipe(serverPipe.get(), nullptr) && GetLastError() != ERROR_PIPE_CONNECTED) {
            std::cerr << "ConnectNamedPipe failed.\n";
            continue;
        }

        PipeMessage request{};
        if (!ReadFromPipe(serverPipe.get(), &request, sizeof(request))) {
            std::cerr << "Failed to read request" << std::endl;
            DisconnectNamedPipe(serverPipe.get());
            continue;
        }

        ServerResponse response{};
        Employee* emp = FindEmployee(employees, request.employeeId);

        if (!emp) {
            response.success = false;
            strncpy_s(response.message, "Employee not found", sizeof(response.message));
        }
        else {
            switch (request.type) {
            case RequestType::Read:
                response.success = true;
                response.record = *emp;
                strncpy_s(response.message, "Read successful", sizeof(response.message));
                break;
            case RequestType::WriteBack:
                *emp = request.record;
                response.success = true;
                strncpy_s(response.message, "Write successful", sizeof(response.message));
                SaveEmployees(filename, employees);
                break;
            case RequestType::Shutdown:
                response.success = true;
                strncpy_s(response.message, "Server shutting down", sizeof(response.message));
                WriteToPipe(serverPipe.get(), &response, sizeof(response));
                DisconnectNamedPipe(serverPipe.get());
                return 0;
            default:
                response.success = false;
                strncpy_s(response.message, "Unknown request", sizeof(response.message));
            }
        }

        WriteToPipe(serverPipe.get(), &response, sizeof(response));
        DisconnectNamedPipe(serverPipe.get());
    }
}
