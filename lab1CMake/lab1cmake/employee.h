#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include <iostream>
#include <cstring>

struct employee {
    int num;
    char name[10];
    double hours;

    double salary(double rate) const { 
        return hours * rate; }

    friend std::ostream& operator<<(std::ostream& os, const employee& e) {
        os << e.num << " " << e.name << " " << e.hours;
        return os;
    }

    friend std::istream& operator>>(std::istream& is, employee& e) {
        is >> e.num;
        is.ignore();
        is.getline(e.name, sizeof(e.name));
        is >> e.hours;
        return is;
    }

    bool operator<(const employee& other) const {
        return num < other.num;
    }

};

#endif

