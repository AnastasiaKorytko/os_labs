#include "sender.h"
#include <iostream>

Sender::Sender(const std::shared_ptr<MessageQueue>& q) : queue_(q) {}

void Sender::doSend()
{
    std::cout << "Enter message (<" << DEFAULT_MAX_MESSAGE_LENGTH << " chars): ";
    std::string msg;
    std::getline(std::cin, msg);
    if (msg.empty()) {
        std::cout << "Empty message, aborted.\n";
        return;
    }
    if (msg.length() > DEFAULT_MAX_MESSAGE_LENGTH) 
        msg.resize(DEFAULT_MAX_MESSAGE_LENGTH);
    if (!queue_->tryPush(msg)) {
        std::cout << "Queue full, message not sent.\n";
    }
    else {
        std::cout << "Message sent.\n";
    }
}

void Sender::run()
{
    std::string command;
    while (true) {
        std::cout << "Enter command (send/exit): ";
        if (!(std::cin >> command)) break;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (command == "exit") break;
        if (command == "send") doSend();
        else std::cout << "Unknown command\n";
    }
}
