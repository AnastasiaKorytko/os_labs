#pragma once
#include <array>
#include <cstddef>
#include <cstdint>

constexpr size_t kMaxNameLen = 10;
constexpr wchar_t kServerPipeName[] = L"\\\\.\\pipe\\Lab5ServerPipe";

enum class RequestType : uint8_t {
    Read = 1,
    Modify = 2,
    Release = 3,
    WriteBack = 4,
    Shutdown = 5
};

struct Employee {
    int id{};
    std::array<char, kMaxNameLen> name{};
    double hours{};
};

struct PipeMessage {
    RequestType type{};
    int employeeId{};
    Employee record{};
    wchar_t clientPipe[256]{};
};

struct ServerResponse {
    bool success{};
    char message[128]{};
    Employee record{};
};
