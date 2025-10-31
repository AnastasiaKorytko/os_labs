#include "marker_system.h"
#include <iostream>
#include <string>

int main() {
    using namespace marker_lab;

    try {
        std::size_t array_size = 0;
        std::cout << ARRAY_SIZE;
        if (!(std::cin >> array_size) || array_size == 0) {
            std::cerr << "Invalid array size" << std::endl;
            return 1;
        }

        MarkerController controller(array_size);
        std::size_t markers = 0;
        std::cout << MARKER_COUNT;
        if (!(std::cin >> markers) || markers == 0) {
            std::cerr << "Invalid number of markers" << std::endl;
            return 1;
        }

        controller.start_markers(markers);
        controller.signal_start_all();

        while (controller.has_active_markers()) {
            controller.wait_all_waiting();
            std::cout << "Array copy before termination:" << std::endl;
            controller.print_array(std::cout);
            std::size_t id = 0;
            while (true) {
                std::cout << "Enter marker id to terminate ";
                if (!(std::cin >> id)) {
                    std::cin.clear();
                    std::string temp;
                    std::getline(std::cin, temp);
                    std::cerr << "Invalid input" << std::endl;
                    continue;
                }
                if (id < 1 || id > markers) {
                    std::cerr << "Out of range" << std::endl;
                    continue;
                }
                break;
            }

            if (!controller.terminate_marker(id)) {
                std::cerr << "Failed to terminate marker " << id << std::endl;
            }

            std::cout << "Array copy after termination:" << std::endl;
            controller.print_array(std::cout);
            controller.signal_continue_all();
        }

        controller.join_all();
        std::cout << "All markers terminated" << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Unhandled exception: " << e.what() << std::endl;
        return 2;
    }

    return 0;
}
