#pragma once
#include "message_queue.h"
#include <memory>
#include <string>

class Receiver {
public:
    Receiver(const std::shared_ptr<MessageQueue>& q);
    void run();

private:
    void doRead();
    std::shared_ptr<MessageQueue> queue_;
};
