#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include <iostream>

struct employee {
    int num;
    char name[10];
    double hours;

    [[nodiscard]] double salary(double rate) const noexcept {
        return hours * rate;
    }

    friend std::ostream& operator<<(std::ostream& os, const employee& e) {
        return os << e.num << " " << e.name << " " << e.hours;
    }

    friend std::istream& operator>>(std::istream& is, employee& e) {
        is >> e.num;
        is.ignore();
        is.get(e.name, 10);
        is >> e.hours;
        return is;
    }

    bool operator<(const employee& other) const noexcept {
        return num < other.num;
    }
};

#endif