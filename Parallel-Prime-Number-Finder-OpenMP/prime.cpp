#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>
#include <omp.h>

using namespace std;

// Returns true when n is a prime number.
bool isPrime(int n) {
    if (n < 2) return false;
    if (n == 2) return true;
    if (n % 2 == 0) return false;

    for (int divisor = 3; divisor <= n / divisor; divisor += 2) {
        if (n % divisor == 0) return false;
    }
    return true;
}

// Checks every number one after another.
vector<int> findPrimesSequential(int first, int last) {
    vector<int> primes;

    for (int number = first; number <= last; ++number) {
        if (isPrime(number)) {
            primes.push_back(number);
        }
    }
    return primes;
}

// Each thread stores its results separately to avoid a race condition.
vector<int> findPrimesStatic(int first, int last) {
    const int threadCount = omp_get_max_threads();
    vector<vector<int>> threadResults(threadCount);

    #pragma omp parallel
    {
        int threadId = omp_get_thread_num();

        #pragma omp for schedule(static)
        for (int number = first; number <= last; ++number) {
            if (isPrime(number)) {
                threadResults[threadId].push_back(number);
            }
        }
    }

    vector<int> primes;
    for (const auto& results : threadResults) {
        primes.insert(primes.end(), results.begin(), results.end());
    }
    sort(primes.begin(), primes.end());
    return primes;
}

// Dynamic scheduling gives another chunk to a thread when it is ready.
vector<int> findPrimesDynamic(int first, int last) {
    const int threadCount = omp_get_max_threads();
    vector<vector<int>> threadResults(threadCount);

    #pragma omp parallel
    {
        int threadId = omp_get_thread_num();

        #pragma omp for schedule(dynamic, 100)
        for (int number = first; number <= last; ++number) {
            if (isPrime(number)) {
                threadResults[threadId].push_back(number);
            }
        }
    }

    vector<int> primes;
    for (const auto& results : threadResults) {
        primes.insert(primes.end(), results.begin(), results.end());
    }
    sort(primes.begin(), primes.end());
    return primes;
}

void printPrimes(const vector<int>& primes) {
    for (size_t i = 0; i < primes.size(); ++i) {
        cout << primes[i];
        if (i + 1 < primes.size()) cout << " ";
    }
    cout << '\n';
}

int main() {
    int first, last;

    cout << "---------------------------------------------\n";
    cout << "       PARALLEL PRIME NUMBER FINDER\n";
    cout << "---------------------------------------------\n";
    cout << "Enter the starting number: ";
    if (!(cin >> first)) {
        cerr << "Please enter a valid integer.\n";
        return 1;
    }

    cout << "Enter the ending number: ";
    if (!(cin >> last)) {
        cerr << "Please enter a valid integer.\n";
        return 1;
    }

    if (first > last) {
        cerr << "Invalid range: starting number must not exceed ending number.\n";
        return 1;
    }

    // Measure each version separately.
    double startTime = omp_get_wtime();
    vector<int> sequential = findPrimesSequential(first, last);
    double sequentialTime = omp_get_wtime() - startTime;

    startTime = omp_get_wtime();
    vector<int> staticResult = findPrimesStatic(first, last);
    double staticTime = omp_get_wtime() - startTime;

    startTime = omp_get_wtime();
    vector<int> dynamicResult = findPrimesDynamic(first, last);
    double dynamicTime = omp_get_wtime() - startTime;

    cout << "\nPrime numbers:\n";
    printPrimes(sequential);
    cout << "\nTotal primes found: " << sequential.size() << '\n';

    cout << fixed << setprecision(6);
    cout << "\nExecution time\n";
    cout << "Sequential       : " << sequentialTime << " seconds\n";
    cout << "OpenMP static    : " << staticTime << " seconds\n";
    cout << "OpenMP dynamic   : " << dynamicTime << " seconds\n";

    cout << "\nSpeedup compared with sequential\n";
    cout << "Static           : "
         << (staticTime > 0.0 ? sequentialTime / staticTime : 0.0) << "x\n";
    cout << "Dynamic          : "
         << (dynamicTime > 0.0 ? sequentialTime / dynamicTime : 0.0) << "x\n";

    cout << "\nOutput check     : "
         << ((sequential == staticResult && sequential == dynamicResult)
                 ? "All three results match"
                 : "Results do not match")
         << '\n';

    cout << "OpenMP max threads: " << omp_get_max_threads() << '\n';
    return 0;
}
