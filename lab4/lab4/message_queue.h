#pragma once
#include <string>
#include <vector>
#include <mutex>
#include <fstream>
#include <condition_variable>
#include <cstddef>

constexpr std::size_t DEFAULT_MAX_MESSAGE_LENGTH = 20;

class MessageQueue {
public:

    MessageQueue(const std::string& filename, std::size_t maxMessages, std::size_t maxMsgLen = DEFAULT_MAX_MESSAGE_LENGTH);
    ~MessageQueue();

    bool push(const std::string& msg);
    bool tryPush(const std::string& msg);
    bool pop(std::string& outMsg);
    bool tryPop(std::string& outMsg);

    MessageQueue(const MessageQueue&) = delete;
    MessageQueue& operator=(const MessageQueue&) = delete;

private:
    bool initFile();
    std::string readSlot(std::size_t slot);
    bool writeSlot(std::size_t slot, const std::string& s);

private:
    std::string filename_;
    std::size_t maxMessages_;
    std::size_t maxMsgLen_;
    std::size_t head_{ 0 };
    std::size_t tail_{ 0 };
    std::size_t count_{ 0 };
    std::fstream file_; 
    std::mutex mtx_;
    std::condition_variable cv_not_full_;
    std::condition_variable cv_not_empty_;
};
