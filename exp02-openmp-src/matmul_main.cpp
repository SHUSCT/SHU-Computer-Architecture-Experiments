#include "xRandom.hpp"
#include "TimeCounter.hpp"
#include <algorithm>
#include <execution>
#include <iostream>
#include <omp.h>
#include <random>
#include <vector>

/**
 * @brief  Matrix multiplication: A * B = C.
 *
 * @param A A matrix with shape (n, m).
 * @param B A matrix with shape (m, k).
 * @param C A matrix with shape (n, k).
 * @param n The number of rows in matrix A and matrix C.
 * @param m The number of columns in matrix A and the number of rows in matrix B.
 * @param k The number of columns in matrix B and the number of columns in matrix C.
 */
void ABpC(float* A, float* B, float* C, int n, int m, int k)
{
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < k; ++j) {
            for (int l = 0; l < m; ++l) {
                C[i * k + j] += A[i * m + l] * B[l * k + j];
            }
        }
    }
}

int main()
{
    constexpr int n = 2000;
    constexpr int m = 1000;
    constexpr int k = 500;
    std::vector<float> A(n * m);
    std::vector<float> B(m * k);
    std::vector<float> C(n * k);

    xRandUniform<float> randGen;
    randGen.setParams(-1.0, 1.0);
    std::ranges::generate(A, randGen);
    std::ranges::generate(B, randGen);
    std::ranges::generate(C, randGen);

    TimeCounter timeCounter;

    // Serial.
    timeCounter.init();
    timeCounter.startCounting();
    for (int i = 0; i < 10; ++i) {
        ABpC(A.data(), B.data(), C.data(), n, m, k);
    }
    timeCounter.endCounting();
    std::cout << std::format("Serial: {} ms\n", timeCounter.msecond());

    // OMP with 2 threads.
    timeCounter.init();
    timeCounter.startCounting();
#pragma omp parallel for num_threads(2)
    for (int i = 0; i < 10; ++i) {
        ABpC(A.data(), B.data(), C.data(), n, m, k);
    }
    timeCounter.endCounting();
    std::cout << std::format("OMP with 2 threads: {} ms\n", timeCounter.msecond());

    // OMP with 4 threads.
    timeCounter.init();
    timeCounter.startCounting();
#pragma omp parallel for num_threads(4)
    for (int i = 0; i < 10; ++i) {
        ABpC(A.data(), B.data(), C.data(), n, m, k);
    }
    timeCounter.endCounting();
    std::cout << std::format("OMP with 4 threads: {} ms\n", timeCounter.msecond());

    // OMP with 8 threads.
    timeCounter.init();
    timeCounter.startCounting();
#pragma omp parallel for num_threads(8)
    for (int i = 0; i < 10; ++i) {
        ABpC(A.data(), B.data(), C.data(), n, m, k);
    }
    timeCounter.endCounting();
    std::cout << std::format("OMP with 8 threads: {} ms\n", timeCounter.msecond());

    // OMP with 16 threads.
    timeCounter.init();
    timeCounter.startCounting();
#pragma omp parallel for num_threads(16)
    for (int i = 0; i < 10; ++i) {
        ABpC(A.data(), B.data(), C.data(), n, m, k);
    }
    timeCounter.endCounting();
    std::cout << std::format("OMP with 16 threads: {} ms\n", timeCounter.msecond());
}
