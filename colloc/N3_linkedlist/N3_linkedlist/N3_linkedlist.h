#pragma once
#include <iostream>
#include <memory>
#include <vector>

struct Node {
    int data;
    std::shared_ptr<Node> next;
    explicit Node(int value) : data(value), next(nullptr) {}
};

std::shared_ptr<Node> create_list(const std::vector<int>& values);

std::shared_ptr<Node> reverse_list(const std::shared_ptr<Node>& head);

void print_list(const std::shared_ptr<Node>& head);

std::vector<int> list_to_vector(const std::shared_ptr<Node>& head);

