# Parallel Prime Number Finder Using OpenMP

## About the project

This is a small C++ program that finds all prime numbers in a range entered by the user. It has three versions of the same task:

- A sequential version, which checks the numbers one at a time.
- An OpenMP version using static scheduling.
- An OpenMP version using dynamic scheduling.

The program prints the prime numbers, counts them, measures the time taken by each version, and checks whether all three versions return the same result.

## Requirements

- A C++ compiler with OpenMP support (GCC is recommended).
- A terminal or command prompt.

## Files

| File | Purpose |
|---|---|
| `prime.cpp` | Main C++ source code |
| `Makefile` | Optional build commands for GCC |
| `.gitignore` | Ignores compiled program files |

## Compile and run

### Linux

```bash
g++ -O2 -std=c++17 -fopenmp prime.cpp -o prime
./prime
```

### macOS

GCC with OpenMP support is required. If GCC is installed through Homebrew, the compiler may be named `g++-14` or another versioned name:

```bash
g++-14 -O2 -std=c++17 -fopenmp prime.cpp -o prime
./prime
```

Use the version of GCC installed on your computer.

### Windows

Install MinGW-w64 with GCC and OpenMP support. In the terminal, run:

```bash
g++ -O2 -std=c++17 -fopenmp prime.cpp -o prime.exe
prime.exe
```

## Example

Input:

```text
Enter the starting number: 1
Enter the ending number: 50
```

The prime numbers should be:

```text
2 3 5 7 11 13 17 19 23 29 31 37 41 43 47
```

There are 15 prime numbers between 1 and 50.

Execution times depend on the computer, compiler, and input range, so your timings will be different from another person's results.

## How the program works

The `isPrime()` function checks whether a number is prime. It only tests odd divisors up to the square root of the number, since a larger factor would have a matching smaller factor.

In the static version, OpenMP divides the loop iterations among the threads before the loop starts. In the dynamic version, threads receive another chunk of 100 iterations when they finish their current chunk.

Each thread writes to its own vector. The vectors are combined after the parallel region, avoiding simultaneous writes to one shared vector.

## Comparing the schedules

- **Static scheduling:** has low scheduling overhead and is useful when loop iterations take roughly similar amounts of time.
- **Dynamic scheduling:** can balance uneven workloads better, but assigning chunks while the loop runs adds overhead.

Neither schedule is guaranteed to be faster for every input. Try a larger range, run the program several times, and compare the timings on the same computer.

## Changing the number of threads

Set the `OMP_NUM_THREADS` environment variable before running the program.

Linux/macOS:

```bash
OMP_NUM_THREADS=4 ./prime
```

Windows Command Prompt:

```bat
set OMP_NUM_THREADS=4
prime.exe
```

## Notes

- The starting and ending values are included in the search.
- Numbers smaller than 2 are not prime.
- The program checks that sequential, static, and dynamic results match.
- Very small ranges may finish faster sequentially because parallel execution has overhead.

## Learning objective

The aim of this program is to understand loop-level parallelism in OpenMP and compare sequential execution with static and dynamic scheduling.
