#include "receiver.h"
#include <iostream>

Receiver::Receiver(const std::shared_ptr<MessageQueue>& q) : queue_(q) {}

void Receiver::doRead()
{
    std::string msg;
    if (queue_->tryPop(msg)) {
        std::cout << "Received: " << msg << std::endl;
    }
    else {
        std::cout << "No message in queue\n";
    }
}

void Receiver::run()
{
    std::string command;
    while (true) {
        std::cout << "Enter command (read/exit): ";
        if (!(std::cin >> command)) break;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (command == "exit") break;
        if (command == "read") doRead();
        else std::cout << "Unknown command\n";
    }
}
