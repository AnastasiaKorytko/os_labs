#include "Array.h"
#include <gtest/gtest.h>
#include <windows.h>
#include "threads.h"

void RunThreadAndWait(DWORD(*threadFunc)(LPVOID), Array* data) {
    HANDLE hThread = CreateThread(NULL, 0, threadFunc, data, 0, NULL);
    WaitForSingleObject(hThread, INFINITE);
    CloseHandle(hThread);
}

TEST(ArrayTest, ReplaceMinElement) {

    int arr[] = { 1, 5, 10, 1 };
    Array data = { arr, 4, 1, 10, 5.5 };

    int avgRound = static_cast<int>(data.avg + 0.5);
    data.makeResArray(avgRound, arr);

    EXPECT_EQ(arr[0], avgRound);
}

TEST(ArrayTest, ReplaceMaxElement) {

    int arr[] = { 1, 5, 10, 1 };
    Array data = { arr, 4, 1, 10, 5.5 };

    int avgRound = static_cast<int>(data.avg + 0.5);
    data.makeResArray(avgRound, arr);

    EXPECT_EQ(arr[2], avgRound);
}

TEST(MinMaxThreadTest, FindCorrectMin) {

    int arr[] = { 7, 2, 9, 4 };
    Array data = { arr, 4, 0, 0, 0.0 };

    RunThreadAndWait(MinMaxThread, &data);

    EXPECT_EQ(data.minVal, 2);
}

TEST(MinMaxThreadTest, FindCorrectMax) {

    int arr[] = { 7, 2, 9, 4 };
    Array data = { arr, 4, 0, 0, 0.0 };

    RunThreadAndWait(MinMaxThread, &data);

    EXPECT_EQ(data.maxVal, 9);
}

TEST(AverageThreadTest, CalculateCorrectAverage) {
 
    int arr[] = { 2, 4, 6, 8 };
    Array data = { arr, 4, 0, 0, 0.0 };

    RunThreadAndWait(AverageThread, &data);

    EXPECT_DOUBLE_EQ(data.avg, 5.0);
}