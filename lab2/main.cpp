#include <iostream>
#include <cmath>

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "Array.h"
#include "threads.h"

int main() {
    std::cout << "Enter the array size ";
    int n;
    std::cin >> n;
    if (!std::cin || n <= 0) {
        std::cerr << "Wrong size " << std::endl;
        return 1;
    }

    int* arr = new int[n];
    std::cout << "Enter " << n << " integers" << std::endl;
    for (int i = 0; i < n; i++) {
        std::cin >> arr[i];
    }

    Array data;
    data.n = n;
    data.arr = arr;
    data.minVal = 0;
    data.maxVal = 0;
    data.avg = 0.0;

    HANDLE hMinMax = CreateThread(NULL, 0, MinMaxThread, &data, 0, NULL);
    if (hMinMax == NULL) {
        std::cerr << "Failed to create thread min_max " << std::endl;
        delete[] arr;
        return 1;
    }

    HANDLE hAverage = CreateThread(NULL, 0, AverageThread, &data, 0, NULL);
    if (hAverage == NULL) {
        std::cerr << "Failed to create thread average " << std::endl;
        CloseHandle(hMinMax);
        delete[] arr;
        return 1;
    }

    WaitForSingleObject(hMinMax, INFINITE);
    WaitForSingleObject(hAverage, INFINITE);

    CloseHandle(hMinMax);
    CloseHandle(hAverage);

    int average = (int)(data.avg + 0.5);

    data.makeResArray(average, arr);

    std::cout << "Result array " << std::endl;
    for (int i = 0; i < n; i++) {
        std::cout << arr[i];
        if (i + 1 < n) {
            std::cout << ' ';
        }
        else {
            std::cout << '\n';
        }
    }

    delete[] arr;
    return 0;
}