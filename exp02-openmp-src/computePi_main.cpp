#include "TimeCounter.hpp"
#include "xRandom.hpp"
#include <format>
#include <iostream>
#include <omp.h>
#include <execution>

double computePi(int n)
{
    double pi = 0.0;
    double step = 1.0 / n;
    for (int i = 0; i < n; ++i) {
        double x = (i + 0.5) * step;
        pi += 4.0 / (1.0 + x * x);
    }
    return pi * step;
}

double computePi_OMP(int n, int num_threads)
{
    double pi = 0.0;
    double step = 1.0 / n;
    omp_set_num_threads(num_threads);
#pragma omp parallel for reduction(+ : pi)
    for (int i = 0; i < n; ++i) {
        double x = (i + 0.5) * step;
        pi += 4.0 /
         (1.0 + x * x);
    }
    return pi * step;
}

double computePi_STL(int n)
{
    double pi = 0.0;
    double step = 1.0 / n;
    std::vector<int> steps(n);
    std::iota(steps.begin(), steps.end(), 0);

    pi = std::transform_reduce(std::execution::par_unseq, steps.begin(), steps.end(), 0.0,
                               std::plus<double>(), [step](int i) {
                                   double x = (i + 0.5) * step;
                                   return 4.0 / (1.0 + x * x);
                               });
    return pi * step;
}



int main()
{
    int n = std::numeric_limits<int>::max();
    TimeCounter timeCounter;

    // Serial.
    timeCounter.init();
    timeCounter.startCounting();
    double pi = computePi(n);
    timeCounter.endCounting();
    std::cout << std::format("Serial: {} ms, pi = {}\n", timeCounter.msecond(), pi);
    int64_t baseline = timeCounter.msecond();

    // OMP with 2 threads.
    timeCounter.init();
    timeCounter.startCounting();
    double pi_OMP = computePi_OMP(n, 2);
    timeCounter.endCounting();
    std::cout << std::format("OMP with 2 threads: {} ms, pi = {}\n", timeCounter.msecond(), pi_OMP);

    // OMP with 4 threads.
    timeCounter.init();
    timeCounter.startCounting();
    pi_OMP = computePi_OMP(n, 4);
    timeCounter.endCounting();
    std::cout << std::format("OMP with 4 threads: {} ms, pi = {}\n", timeCounter.msecond(), pi_OMP);

    // OMP with 8 threads.
    timeCounter.init();
    timeCounter.startCounting();
    pi_OMP = computePi_OMP(n, 8);
    timeCounter.endCounting();
    std::cout << std::format("OMP with 8 threads: {} ms, pi = {}\n", timeCounter.msecond(), pi_OMP);

    // OMP with 16 threads.
    timeCounter.init();
    timeCounter.startCounting();
    pi_OMP = computePi_OMP(n, 16);
    timeCounter.endCounting();
    std::cout << std::format("OMP with 16 threads: {} ms, pi = {}\n", timeCounter.msecond(),
                             pi_OMP);

    // OMP STL
    omp_set_num_threads(1);
    timeCounter.init();
    timeCounter.startCounting();
    pi_OMP = computePi_STL(n);
    timeCounter.endCounting();
    std::cout << std::format("OMP with STL: {} ms, pi = {}\n", timeCounter.msecond(),
                             pi_OMP);

    double accleration = double(baseline) / timeCounter.msecond();
    std::cout << std::format("Accleration ratio: {}", accleration);

    return 0;
}