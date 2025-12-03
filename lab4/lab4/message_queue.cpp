#include "message_queue.h"
#include <iostream>
#include <cstring>
#include <algorithm>

MessageQueue::MessageQueue(const std::string& filename, std::size_t maxMessages, std::size_t maxMsgLen)
    : filename_(filename), maxMessages_(maxMessages), maxMsgLen_(maxMsgLen)
{
    if (maxMessages_ == 0 || maxMsgLen_ == 0) {
        throw std::invalid_argument("maxMessages and maxMsgLen must be > 0");
    }
    if (!initFile()) {
        throw std::runtime_error("Failed to initialize queue file: " + filename_);
    }
}

MessageQueue::~MessageQueue()
{
    std::lock_guard<std::mutex> lk(mtx_);
    if (file_.is_open()) file_.close();
}

bool MessageQueue::initFile()
{
    std::ofstream ofs(filename_, std::ios::binary | std::ios::trunc);
    if (!ofs.is_open()) return false;

    std::string empty(maxMsgLen_, '\0');
    for (std::size_t i = 0; i < maxMessages_; ++i) {
        ofs.write(empty.data(), static_cast<std::streamsize>(empty.size()));
        if (!ofs) return false;
    }
    ofs.close();

    file_.open(filename_, std::ios::binary | std::ios::in | std::ios::out);
    return file_.is_open();
}

bool MessageQueue::writeSlot(std::size_t slot, const std::string& s)
{
    if (!file_.is_open()) return false;
    if (slot >= maxMessages_) return false;

    std::string tmp = s;
    if (tmp.size() > maxMsgLen_) tmp.resize(maxMsgLen_);
    else if (tmp.size() < maxMsgLen_) tmp.append(maxMsgLen_ - tmp.size(), '\0');

    std::streampos pos = static_cast<std::streampos>(slot * maxMsgLen_);
    file_.seekp(pos);
    file_.write(tmp.data(), static_cast<std::streamsize>(tmp.size()));
    file_.flush();
    return static_cast<bool>(file_);
}

std::string MessageQueue::readSlot(std::size_t slot)
{
    std::string buf(maxMsgLen_, '\0');
    if (!file_.is_open()) return {};

    std::streampos pos = static_cast<std::streampos>(slot * maxMsgLen_);
    file_.seekg(pos);
    file_.read(&buf[0], static_cast<std::streamsize>(maxMsgLen_));
    auto endpos = std::find_if(buf.rbegin(), buf.rend(), [](char c) { return c != '\0'; });
    buf.erase(endpos.base(), buf.end());
    return buf;
}

bool MessageQueue::tryPush(const std::string& msg)
{
    std::lock_guard<std::mutex> lk(mtx_);
    if (count_ >= maxMessages_) return false;
    if (!writeSlot(tail_, msg)) return false;
    tail_ = (tail_ + 1) % maxMessages_;
    ++count_;
    cv_not_empty_.notify_one();
    return true;
}

bool MessageQueue::push(const std::string& msg)
{
    std::unique_lock<std::mutex> lk(mtx_);
    cv_not_full_.wait(lk, [this] { return count_ < maxMessages_; });
    if (!writeSlot(tail_, msg)) return false;
    tail_ = (tail_ + 1) % maxMessages_;
    ++count_;
    lk.unlock();
    cv_not_empty_.notify_one();
    return true;
}

bool MessageQueue::tryPop(std::string& outMsg)
{
    std::lock_guard<std::mutex> lk(mtx_);
    if (count_ == 0) return false;
    outMsg = readSlot(head_);
    head_ = (head_ + 1) % maxMessages_;
    --count_;
    cv_not_full_.notify_one();
    return true;
}

bool MessageQueue::pop(std::string& outMsg)
{
    std::unique_lock<std::mutex> lk(mtx_);
    cv_not_empty_.wait(lk, [this] { return count_ > 0; });
    outMsg = readSlot(head_);
    head_ = (head_ + 1) % maxMessages_;
    --count_;
    lk.unlock();
    cv_not_full_.notify_one();
    return true;
}
