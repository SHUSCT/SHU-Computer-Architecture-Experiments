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

int main()
{
    size_t a = 10240, b = 10240, c = 10240;
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

    std::cout << std::format("Time cost: {}", counter.msecond());
}