#include "receiver.h"
#include "message_queue.h"
#include <iostream>
#include <memory>

int main()
{
    try {
        std::string filename;
        size_t maxMessages;

        std::cout << "Enter filename: ";
        std::cin >> filename;
        std::cout << "Enter number of messages: ";
        std::cin >> maxMessages;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        auto queue = std::make_shared<MessageQueue>(filename, maxMessages, DEFAULT_MAX_MESSAGE_LENGTH);
        Receiver r(queue);
        r.run();
    }
    catch (const std::exception& ex) {
        std::cerr << "Fatal error: " << ex.what() << std::endl;
        return 1;
    }
    return 0;
}
