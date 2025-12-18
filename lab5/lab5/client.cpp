#include "common.h"
#include <windows.h>
#include <iostream>
#include <string>
#include <algorithm>
#include <memory>

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

int main() {
    std::cout << "Client" << std::endl;

    PipeHandle clientPipe(CreateFileW(
        kServerPipeName,
        GENERIC_READ | GENERIC_WRITE,
        0, nullptr, OPEN_EXISTING, 0, nullptr));

    if (clientPipe.get() == INVALID_HANDLE_VALUE) {
        std::cerr << "Failed to connect to server pipe" << std::endl;
        return 1;
    }

    while (true) {
        std::cout << "Command (r=read, m=modify, s=shutdown, e=exit): ";
        std::string cmd; std::getline(std::cin, cmd);
        if (cmd.empty()) continue;

        PipeMessage msg{};
        if (cmd[0] == 'e') break;

        if (cmd[0] == 's') {
            msg.type = RequestType::Shutdown;
            WriteToPipe(clientPipe.get(), &msg, sizeof(msg));
        }
        else {
            int id{};
            std::cout << "Employee ID: "; std::cin >> id; std::cin.ignore();
            msg.employeeId = id;

            if (cmd[0] == 'r') msg.type = RequestType::Read;
            else if (cmd[0] == 'm') {
                msg.type = RequestType::WriteBack;
                Employee e{};

                std::cout << "New name: ";
                std::string name; std::getline(std::cin, name);
                std::fill(e.name.begin(), e.name.end(), 0);
                size_t max_copy = e.name.size() - 1;
                size_t copy_len = (name.size() < max_copy) ? name.size() : max_copy;
                for (size_t i = 0; i < copy_len; ++i) e.name[i] = name[i];

                std::cout << "New hours: "; std::cin >> e.hours; std::cin.ignore();
                msg.record = e;
            }
            else {
                std::cout << "Unknown command" << std::endl;
                continue;
            }

            WriteToPipe(clientPipe.get(), &msg, sizeof(msg));
        }

        ServerResponse resp{};
        if (!ReadFromPipe(clientPipe.get(), &resp, sizeof(resp))) {
            std::cerr << "Failed to read response" << std::endl;
            continue;
        }

        std::cout << "Server: " << resp.message << std::endl;
        if (resp.success && msg.type == RequestType::Read) PrintEmployee(resp.record);
    }

    return 0;
}
