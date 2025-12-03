#pragma once
#include "message_queue.h"
#include <memory>
#include <string>

class Sender {
public:
    explicit Sender(const std::shared_ptr<MessageQueue>& q);
    void run(); 

private:
    void doSend();
    std::shared_ptr<MessageQueue> queue_;
};
