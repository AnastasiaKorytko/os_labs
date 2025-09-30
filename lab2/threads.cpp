#include <iostream>
#include "threads.h"

DWORD WINAPI MinMaxThread(LPVOID lpParam) {
    Array* data = (Array*)lpParam;
    if (data->n <= 0 || data->arr == 0) {
        return 1;
    }

    int currMin = data->arr[0];
    int currMax = data->arr[0];

    for (int i = 1; i < data->n; ++i) {
        if (data->arr[i] < currMin) {
            currMin = data->arr[i];
        }
        Sleep(7);

        if (data->arr[i] > currMax) {
            currMax = data->arr[i];
        }
        Sleep(7);
    }

    data->minVal = currMin;
    data->maxVal = currMax;

    std::cout << "min = " << data->minVal << ", max = " << data->maxVal << std::endl;

    return 0;
}

DWORD WINAPI AverageThread(LPVOID lpParam) {
    Array* data = (Array*)lpParam;
    if (data->n <= 0 || data->arr == 0) {
        return 1;
    }

    long long sum = 0;
    for (int i = 0; i < data->n; ++i) {
        sum += data->arr[i];
        Sleep(12);
    }

    data->avg = (double)sum / (double)data->n;

    std::cout << "average = " << data->avg << std::endl;

    return 0;
}