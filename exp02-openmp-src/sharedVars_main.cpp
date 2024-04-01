#include "TimeCounter.hpp"
#include <format>
#include <iostream>
#include <omp.h>

template <typename T>
concept Addable = requires(T a, T b) {
    { a + b } -> std::convertible_to<T>;
};

enum class ExecutionPolicy {
    seq,
    par,
    atomic,
    critical,
    par_reduce,
};

template <typename T, ExecutionPolicy policy>
    requires Addable<T>
T accumulate(const T& start, const T& end) {
    T sum = 0;
    if constexpr (ExecutionPolicy::seq == policy) {
        for (T i = start; i < end; ++i) {
            sum += i;
        }
    } else if constexpr (ExecutionPolicy::par == policy) {
#pragma omp parallel for
        for (T i = start; i < end; ++i) {
            sum += i;
        }
    } else if constexpr (ExecutionPolicy::atomic == policy) {
#pragma omp parallel for
        for (T i = start; i < end; ++i) {
#pragma omp atomic
            sum += i;
        }
    } else if constexpr (ExecutionPolicy::critical == policy) {
#pragma omp parallel for
        for (T i = start; i < end; ++i) {
#pragma omp critical
            { sum += i; }
        }
    } else if constexpr (ExecutionPolicy::par_reduce == policy) {
#pragma omp parallel for reduction(+ : sum)
        for (T i = start; i < end; ++i) {
            sum += i;
        }
    }
    return sum;
}

int main() {
    TimeCounter timeCounter;
    constexpr int n = std::numeric_limits<int>::max() >> 2;

    // Serial.
    timeCounter.init();
    timeCounter.startCounting();
    int sum = accumulate<int, ExecutionPolicy::seq>(0, n);
    timeCounter.endCounting();
    std::cout << std::format("Serial: {} ms, sum = {}\n", timeCounter.msecond(), sum);

    // OpenMP.
    timeCounter.init();
    timeCounter.startCounting();
    sum = accumulate<int, ExecutionPolicy::par>(0, n);
    timeCounter.endCounting();
    std::cout << std::format("OpenMP: {} ms, sum = {}\n", timeCounter.msecond(), sum);

    // OpenMP atomic.
    timeCounter.init();
    timeCounter.startCounting();
    sum = accumulate<int, ExecutionPolicy::atomic>(0, n);
    timeCounter.endCounting();
    std::cout << std::format("OpenMP atomic: {} ms, sum = {}\n", timeCounter.msecond(), sum);

    // OpenMP critical.
    timeCounter.init();
    timeCounter.startCounting();
    sum = accumulate<int, ExecutionPolicy::critical>(0, n);
    timeCounter.endCounting();
    std::cout << std::format("OpenMP critical: {} ms, sum = {}\n", timeCounter.msecond(), sum);

    // OpenMP reduce.
    timeCounter.init();
    timeCounter.startCounting();
    sum = accumulate<int, ExecutionPolicy::par_reduce>(0, n);
    timeCounter.endCounting();
    std::cout << std::format("OpenMP reduce: {} ms, sum = {}\n", timeCounter.msecond(), sum);

    return 0;
}