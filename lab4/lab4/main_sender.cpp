#include "sender.h"
#include "message_queue.h"
#include <iostream>
#include <memory>

int main()
{
    try {
        std::string filename;
        std::cout << "Enter filename: ";
        std::cin >> filename;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        auto queue = std::make_shared<MessageQueue>(filename, 10, DEFAULT_MAX_MESSAGE_LENGTH);
        Sender s(queue);
        s.run();
    }
    catch (const std::exception& ex) {
        std::cerr << "Fatal error: " << ex.what() << std::endl;
        return 1;
    }
    return 0;
}
