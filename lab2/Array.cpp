#include "Array.h"

void Array::makeResArray(int avg, int arr[]) {
    for (int i = 0; i < n; i++) {
        if (arr[i] == minVal || arr[i] == maxVal) {
            arr[i] = avg;
        }
    }
}
