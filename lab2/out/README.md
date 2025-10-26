# Lab #2 — Creating Threads

The program performs multithreaded calculations on an array of numbers:
- Finding the minimum and maximum elements;
- Calculating average values;
- Replacing the minimum and maximum elements with the average.

## Implemented
- **`Array`` class** — stores the array, as well as the calculated values ​​of `minVal`, `maxVal`, and `avg`.
- **Threads:**
- `MinMaxThread` — finds creatives and maximum elements;
- `AverageThread` — calculates the average value;
- `ReplaceThread` — replaces properties and maximum elements with (rounded) values.
- **`Main`` program** — creates threads, synchronizes their execution, and outputs the final result.
- **Unit tests** — implemented using GoogleTest:
- checking the correctness of minimum and maximum calculations;
- checking the correctness of average values;
- checking the correctness of element replacements.

## Standard
- C++98
