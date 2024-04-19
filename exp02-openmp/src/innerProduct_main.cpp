#include "InnerProduct.hpp"
#include <algorithm>
#include <climits>
#include <format>
#include <iostream>
#include <omp.h>
#include <string>
#include "Yutils/TimeCounter.hpp"
#include "Yutils/Random.hpp"

using yutils::TimeCounter;
using yutils::RandUniform;

enum class ExecutionPolicy
{
    seq,
    par,
    cuda,
};

template <ExecutionPolicy policy>
void innerProduct(float* vecA, float* vecB, float* vecRes, int n)
{
    if constexpr (policy == ExecutionPolicy::seq) {
        for (int i = 0; i < n; ++i) {
            vecRes[i] = vecA[i] * vecB[i];
        }
    } else if constexpr (policy == ExecutionPolicy::par) {
#pragma omp parallel for
        for (int i = 0; i < n; ++i) {
            vecRes[i] = vecA[i] * vecB[i];
        }
    } else if constexpr (policy == ExecutionPolicy::cuda) {
        cuInnerProduct(vecA, vecB, vecRes, n);
    }
}

int main(int argc, char* argv[])
{
    int n = std::numeric_limits<int>::max();
    if (argc > 1) {
        n = std::stoi(argv[1]);
    }

    std::vector<float> vecA(n, 0);
    std::vector<float> vecB(n, 0);

    RandUniform<float> randGen;
    randGen.setParams(-1.0, 1.0);

    std::ranges::generate(vecA, randGen);
    std::ranges::generate(vecB, randGen);

    std::vector<float> groundTruth(n, 0);

    TimeCounter counter;

    {
        counter.init();
        std::vector<float> vecRes(n, 0);
        counter.startCounting();
        innerProduct<ExecutionPolicy::seq>(vecA.data(), vecB.data(), vecRes.data(), n);
        counter.endCounting();
        std::cout << std::format("Sequential time: {}ms\n", counter.msecond());
        groundTruth = vecRes;
    }

    {
        counter.init();
        std::vector<float> vecRes(n, 0);
        counter.startCounting();
        innerProduct<ExecutionPolicy::par>(vecA.data(), vecB.data(), vecRes.data(), n);
        counter.endCounting();
        std::cout << std::format("Parallel time: {}ms\n", counter.msecond());
        if (vecRes == groundTruth) {
            std::cout << "Parallel result is correct\n";
        } else {
            std::cout << "Parallel result is incorrect\n";
        }
    }

    {
        counter.init();
        std::vector<float> vecRes(n, 0);
        counter.startCounting();
        innerProduct<ExecutionPolicy::cuda>(vecA.data(), vecB.data(), vecRes.data(), n);
        counter.endCounting();
        std::cout << std::format("CUDA time: {}ms\n", counter.msecond());
        if (vecRes == groundTruth) {
            std::cout << "CUDA result is correct\n";
        } else {
            std::cout << "CUDA result is incorrect\n";
        }
    }

    return 0;
}