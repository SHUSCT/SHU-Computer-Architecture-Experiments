#pragma once
#include <chrono>
#include <numeric>
#include <string>
#include <vector>

class TimeCounter
{
public:
    explicit TimeCounter() = default;
    TimeCounter& operator=(const TimeCounter&) = delete;
    ~TimeCounter() = default;

public:
    // Clear the content in {resultList}.
    void init()
    {
        resultList.clear();
        whetherStart = false;
    }

    // Clear the content in {resultList}.
    void reset()
    {
        resultList.clear();
        whetherStart = false;
    }

    // Start counting.
    void startCounting()
    {
        start = std::chrono::high_resolution_clock::now();
        whetherStart = true;
    }

    // Stop counting and store duration into {resultList}.
    void endCounting()
    {
        if (!whetherStart) {
            throw std::runtime_error("End Counting before starting.");
        }
        auto end = std::chrono::high_resolution_clock::now();
        resultList.push_back(end - start);
        whetherStart = false;
    }

    // Return average duration in millisecond.
    int64_t msecond()
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>(calAverage()).count();
    }

    // Return average duration in second.
    double second()
    {
        return std::chrono::duration_cast<std::chrono::duration<double>>(calAverage()).count();
    }

private:
    std::chrono::high_resolution_clock::duration calAverage()
    {
        auto sum = std::accumulate(resultList.begin(), resultList.end(),
                                   std::chrono::high_resolution_clock::duration::zero());
        return sum / resultList.size();
    }

    bool whetherStart = false;
    std::chrono::high_resolution_clock::time_point start;
    std::vector<std::chrono::high_resolution_clock::duration> resultList;
};