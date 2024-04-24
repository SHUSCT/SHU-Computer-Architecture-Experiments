#include <algorithm>
#include <iostream>
#include <mpi.h>
#include <sstream>
#include <vector>

#include <Yutils/ArgParser.hpp>
#include <Yutils/Logger.hpp>
#include <Yutils/Random.hpp>
#include <Yutils/SimpleWriter.hpp>
#include <Yutils/TimeCounter.hpp>

/**
 * @brief Multiply two matrices A and B, and store the result in matrix C.
 * 
 * @param A Matrix A, with shape (n, m)
 * @param B Matrix B, with shape (m, m)
 * @param C Matrix C, with shape (n, m)
*/
void matMult(double* A, double* B, double* C, int n, int m) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            C[i * m + j] = 0;
            for (int k = 0; k < m; k++) {
                C[i * m + j] += A[i * m + k] * B[k * m + j];
            }
        }
    }
}

/**
 * @brief Multiply two matrices A and B, and store the result in matrix C.
 * 
 * @param A Matrix A, with shape (n, m)
 * @param B Matrix B, with shape (m, m)
 * @param C Matrix C, with shape (n, m)
*/
void matMult_OMP(double* A, double* B, double* C, int n, int m) {
    #pragma omp parallel for
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            C[i * m + j] = 0;
            for (int k = 0; k < m; k++) {
                C[i * m + j] += A[i * m + k] * B[k * m + j];
            }
        }
    }
}

void initMat(std::vector<double>& mat)
{
    yutils::RandUniform<double> randGen;
    randGen.setParams(0.0, 10.0);
    std::ranges::generate(mat, randGen);
}

int main(int argc, char** argv)
{
    int rank,    // rank of the current process
        nRanks;  // total number of processes

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nRanks);

    yutils::ArgParser parser;
    parser.addOption("n", "Matrix Size", "int", "4");
    parser.addOption("o", "Output File Path", "string", "output.csv");
    parser.parse(argc, argv);
    auto matSize = parser.get<int>("n").value();
    auto outPath = parser.get<std::string>("o").value();

    yutils::TimeCounter timeCounter;

    std::vector<double> a(matSize * matSize, 0), b(matSize * matSize, 0), c(matSize * matSize, 0),
        sub_a(matSize * matSize / nRanks), sub_c(matSize * matSize / nRanks);

    if (rank == 0) {
        initMat(a);
        initMat(b);
        timeCounter.init();
        timeCounter.startCounting();
    }

    MPI_Scatter(a.data(), matSize * matSize / nRanks, MPI_DOUBLE, sub_a.data(),
                matSize * matSize / nRanks, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(b.data(), b.size(), MPI_DOUBLE, 0, MPI_COMM_WORLD);
    matMult(sub_a.data(), b.data(), sub_c.data(), matSize / nRanks, matSize);
    MPI_Gather(sub_c.data(), matSize * matSize / nRanks, MPI_DOUBLE, c.data(),
               matSize * matSize / nRanks, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        timeCounter.endCounting();
        // std::string row;
        // for (int i = 0; i < matSize; i++) {
        //     row.clear();
        //     for (int j = 0; j < matSize; j++) {
        //         row += std::to_string(c[i * matSize + j]);
        //         if (j != matSize - 1) {
        //             row += ",";
        //         }
        //     }
        //     std::cout << row << std::endl;
        // }
        YCRITICAL("Time taken: {}ms", timeCounter.msecond());
    }

    MPI_Finalize();

    return 0;
}
