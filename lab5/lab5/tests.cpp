#include <gtest/gtest.h>
#include "common.h"

#include <string>
#include <vector>
#include <cstring>
#include <algorithm>

bool CopyNameSafe(Employee& e, const std::string& name)
{
    std::fill(e.name.begin(), e.name.end(), 0);

    size_t maxCopy = e.name.size() - 1;
    size_t copyLen = std::min(name.size(), maxCopy);

    for (size_t i = 0; i < copyLen; ++i)
        e.name[i] = name[i];

    return true;
}

struct FakePipe {
    std::vector<char> buffer;
};

bool FakeWrite(FakePipe& p, const void* data, size_t size)
{
    const char* bytes = static_cast<const char*>(data);
    p.buffer.assign(bytes, bytes + size);
    return true;
}

bool FakeRead(FakePipe& p, void* data, size_t size)
{
    if (p.buffer.size() < size)
        return false;

    std::memcpy(data, p.buffer.data(), size);
    return true;
}


TEST(EmployeeTest, CopyName_TruncatesAndNullTerminates)
{
    Employee e{};
    std::string longName = "ABCDEFGHIJKL";

    CopyNameSafe(e, longName);

    EXPECT_EQ(e.name[kMaxNameLen - 1], '\0');
    EXPECT_EQ(std::string(e.name.data()), "ABCDEFGHI");
}

TEST(EmployeeTest, CopyName_ShortNameCopied)
{
    Employee e{};
    CopyNameSafe(e, "Bob");

    EXPECT_EQ(std::string(e.name.data()), "Bob");
}

TEST(PipeMessageTest, SerializationRoundTrip)
{
    PipeMessage msg{};
    msg.type = RequestType::Modify;
    msg.employeeId = 42;

    Employee emp{};
    emp.id = 42;
    emp.hours = 10.5;
    CopyNameSafe(emp, "Alice");
    msg.record = emp;

    FakePipe pipe;
    EXPECT_TRUE(FakeWrite(pipe, &msg, sizeof(msg)));

    PipeMessage restored{};
    EXPECT_TRUE(FakeRead(pipe, &restored, sizeof(restored)));

    EXPECT_EQ(restored.employeeId, 42);
    EXPECT_EQ(restored.record.id, 42);
    EXPECT_DOUBLE_EQ(restored.record.hours, 10.5);
    EXPECT_EQ(std::string(restored.record.name.data()), "Alice");
}

TEST(ServerResponseTest, SerializationRoundTrip)
{
    ServerResponse resp{};
    resp.success = true;
    strncpy_s(resp.message, "OK", sizeof(resp.message));

    Employee emp{};
    emp.id = 7;
    emp.hours = 5.25;
    CopyNameSafe(emp, "Tom");
    resp.record = emp;

    FakePipe pipe;
    EXPECT_TRUE(FakeWrite(pipe, &resp, sizeof(resp)));

    ServerResponse restored{};
    EXPECT_TRUE(FakeRead(pipe, &restored, sizeof(restored)));

    EXPECT_TRUE(restored.success);
    EXPECT_EQ(restored.record.id, 7);
    EXPECT_DOUBLE_EQ(restored.record.hours, 5.25);
    EXPECT_EQ(std::string(restored.record.name.data()), "Tom");
    EXPECT_STREQ(restored.message, "OK");
}

TEST(FakePipeTest, ReadFailsOnSmallBuffer)
{
    FakePipe pipe;
    int x = 123;
    FakeWrite(pipe, &x, sizeof(x));

    PipeMessage msg{};
    EXPECT_FALSE(FakeRead(pipe, &msg, sizeof(msg)));
}
