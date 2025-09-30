#ifndef ARRAY_H
#define ARRAY_H

struct Array {
    int* arr;
    int n;
    int minVal;
    int maxVal;
    double avg;

    void makeResArray(int avg, int arr[]);
};

#endif