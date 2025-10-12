#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <iostream>
#include <vector>
#include <cmath>
#include "Array.h"
#include "threads.h"

int main() {
    std::cout << "Enter the array size: ";
    int n;
    if (!(std::cin >> n) || n <= 0) {
        std::cerr << "Wrong size" << std::endl;
        return 1;
    }
    std::vector<int> v(n);
    std::cout << "Enter " << n << " integers:" << std::endl;
    for (int i = 0; i < n; ++i) std::cin >> v[i];

    Array data(v);

    HANDLE hMinMax = CreateThread(NULL, 0, MinMaxThread, &data, 0, NULL);
    if (hMinMax == NULL) {
        std::cerr << "Failed to create thread min_max" << std::endl;
        return 1;
    }
    HANDLE hAverage = CreateThread(NULL, 0, AverageThread, &data, 0, NULL);
    if (hAverage == NULL) {
        std::cerr << "Failed to create thread average" << std::endl;
        CloseHandle(hMinMax);
        return 1;
    }

    WaitForSingleObject(hMinMax, INFINITE);
    WaitForSingleObject(hAverage, INFINITE);
    CloseHandle(hMinMax);
    CloseHandle(hAverage);

    int roundedAvg = static_cast<int>(data.avg + 0.5);
    data.makeResArray(roundedAvg);

    std::cout << "Result array:" << std::endl;
    for (int i = 0; i < data.n; ++i) {
        std::cout << data.arr[i] << (i + 1 < data.n ? " " : "\n");
    }
    return 0;
}
