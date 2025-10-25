#include "N3_linkedlist.h"
#include <vector>
#include <stdexcept>

std::shared_ptr<Node> create_list(const std::vector<int>& values) {
    if (values.empty())
        return nullptr;
    auto head = std::make_shared<Node>(values[0]);
    auto current = head;
    for (size_t i = 1; i < values.size(); i++) {
        current->next = std::make_shared<Node>(values[i]);
        current = current->next;
    }
    return head;
}

std::shared_ptr<Node> reverse_list(const std::shared_ptr<Node>& head) {
    if (!head || !head->next) {
        return head;
    }
    auto newHead = reverse_list(head->next);
    head->next->next = head;
    head->next = nullptr;
    return newHead;
}

void print_list(const std::shared_ptr<Node>& head) {
    auto current = head;
    while (current) {
        std::cout << current->data << " ";
        current = current->next;
    }
    std::cout << std::endl;
}

std::vector<int> list_to_vector(const std::shared_ptr<Node>& head) {
    std::vector<int> result;
    auto current = head;
    while (current) {
        result.push_back(current->data);
        current = current->next;
    }
    return result;
}
