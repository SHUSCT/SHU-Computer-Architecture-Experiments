#include <algorithm>
#include <cstdint>
#include <format>
#include <iostream>
#include <vector>

#include "Yutils/Random.hpp"
#include "Yutils/TimeCounter.hpp"

using yutils::RandUniform;
using yutils::TimeCounter;

extern "C"
{
    void cudaABpC(float* A, float* B, float* C, size_t a, size_t b, size_t c);
}

void ompABpC(float* A, float* B, float* C, int n, int m, int k)
{
#pragma omp parallel for
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
    size_t a = 2048, b = 2048, c = 2048;
    std::vector<float> A(a * b), B(b * c), C(a * c);

    RandUniform<float> randGen;
    randGen.setParams(-1.0, 1.0);

    std::ranges::generate(A, randGen);
    std::ranges::generate(B, randGen);

    TimeCounter counter;
    counter.init();
    counter.startCounting();
    cudaABpC(A.data(), B.data(), C.data(), a, b, c);
    counter.endCounting();

    std::cout << std::format("CUDA - Time cost: {}ms\n", counter.msecond());

    C.resize(a * c, 0.0f);

    counter.init();
    counter.startCounting();
    ompABpC(A.data(), B.data(), C.data(), a, b, c);
    counter.endCounting();

    std::cout << std::format("OpenMP - Time cost: {}ms\n", counter.msecond());
}