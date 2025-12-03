#include "gtest/gtest.h"
#include "message_queue.h"
#include <filesystem>
#include <memory>
#include <string>


std::shared_ptr<MessageQueue> createQueue(
    const std::string& filename,
    size_t maxMessages,
    size_t maxMsgLength = 32)
{
    if (std::filesystem::exists(filename))
        std::filesystem::remove(filename);

    return std::make_shared<MessageQueue>(filename, maxMessages, maxMsgLength);
}


TEST(MessageQueueTest, EmptyQueue_PopFails)
{
    auto q = createQueue("mq_test_1.dat", 5);
    std::string msg;
    bool ok = q->tryPop(msg);

    EXPECT_FALSE(ok);
}

TEST(MessageQueueTest, PushPopSingleMessage)
{
    auto q = createQueue("mq_test_2.dat", 5);
    std::string input = "Hello";

    bool pushOk = q->tryPush(input);

    std::string output;
    bool popOk = q->tryPop(output);

    EXPECT_TRUE(pushOk);
    EXPECT_TRUE(popOk);
    EXPECT_EQ(output, input);
}

TEST(MessageQueueTest, FIFOOrder)
{
    auto q = createQueue("mq_test_3.dat", 5);
    std::vector<std::string> messages = { "A", "B", "C" };

    for (const auto& m : messages)
        q->tryPush(m);

    std::vector<std::string> result;
    for (int i = 0; i < 3; i++)
    {
        std::string out;
        bool ok = q->tryPop(out);

        EXPECT_TRUE(ok);
        result.push_back(out);
    }

    EXPECT_EQ(result, messages);

    std::string dummy;
    EXPECT_FALSE(q->tryPop(dummy));
}

TEST(MessageQueueTest, MaxSizeLimit)
{
    size_t max = 3;
    auto q = createQueue("mq_test_4.dat", max);

    std::vector<std::string> messages = { "A", "B", "C", "D" };

    for (size_t i = 0; i < messages.size(); i++)
    {
        bool ok = q->tryPush(messages[i]);

        if (i < max)
            EXPECT_TRUE(ok);
        else
            EXPECT_FALSE(ok);
    }
}

TEST(MessageQueueTest, LongMessageIsTruncated)
{
    size_t maxLen = 20;
    auto q = createQueue("mq_test_5.dat", 5, maxLen);

    std::string longMsg(60, 'x');

    bool ok = q->tryPush(longMsg);

    std::string out;
    bool popOk = q->tryPop(out);

    EXPECT_TRUE(ok);
    EXPECT_TRUE(popOk);
    EXPECT_EQ(out.size(), maxLen);
}

TEST(MessageQueueTest, RingBufferWrapsCorrectly)
{
    auto q = createQueue("mq_test_6.dat", 3);

    q->tryPush("1");
    q->tryPush("2");
    q->tryPush("3");

    std::string out;

    q->tryPop(out);
    q->tryPush("4");

    q->tryPop(out); EXPECT_EQ(out, "2");
    q->tryPop(out); EXPECT_EQ(out, "3");
    q->tryPop(out); EXPECT_EQ(out, "4");
}

TEST(MessageQueueTest, BlockingPopWaitsUntilMessageArrives)
{
    auto q = createQueue("mq_test_7.dat", 5);
    std::string out;

    std::thread reader([&]() {
        q->pop(out);
        });

    std::this_thread::sleep_for(std::chrono::milliseconds(150));
    EXPECT_TRUE(out.empty());

    q->push("MSG");
    reader.join();

    EXPECT_EQ(out, "MSG");
}

TEST(MessageQueueTest, BlockingPushWaitsForFreeSlot)
{
    auto q = createQueue("mq_test_8.dat", 1);
    q->push("A");
    auto start = std::chrono::steady_clock::now();

    std::thread t([&]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(150));

        std::string tmp;
        q->pop(tmp);
        });

    q->push("B");
    t.join();

    auto end = std::chrono::steady_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    EXPECT_GE(ms.count(), 120);
}
