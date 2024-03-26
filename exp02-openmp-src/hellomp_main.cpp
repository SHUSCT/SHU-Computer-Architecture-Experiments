#include <format>
#include <iostream>
#include <omp.h>

int main()
{
    int nthreads, tid;       // nthreads: number of threads, tid: thread id
    omp_set_num_threads(8);  // set number of threads to 8
    #pragma omp parallel private(tid)
    {
        tid = omp_get_thread_num();  // get thread id
        std::cout << std::format("Hello World from thread = {}\n", tid);
        if (tid == 0) {
            nthreads = omp_get_num_threads();  // get number of threads
            std::cout << std::format("Number of threads = {}\n", nthreads);
        }
    }
}