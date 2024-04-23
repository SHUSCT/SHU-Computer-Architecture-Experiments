#include <algorithm>
#include <iostream>
#include <mpi.h>
#include <sstream>
#include <vector>

#include <Yutils/ArgParser.hpp>
#include <Yutils/Logger.hpp>
#include <Yutils/Random.hpp>
#include <Yutils/SimpleWriter.hpp>

void matMult(double* a, double* b, double* c, int nCol, int nRow) {
    for (int i = 0; i < nCol; i++) {
        for (int j = 0; j < nRow; j++) {
            c[i * nRow + j] = 0;
            for (int k = 0; k < nRow; k++) {
                c[i * nRow + j] += a[i * nRow + k] * b[k * nRow + j];
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
    int matSize = *parser.get<int>("n");
    std::string outPath = *parser.get<std::string>("o");

    std::vector<double> a(matSize * matSize, 0), b(matSize * matSize, 0), c(matSize * matSize, 0),
        sub_a(matSize * matSize / nRanks), sub_c(matSize * matSize / nRanks);

    if (rank == 0) {
        initMat(a);
        initMat(b);
    }

    MPI_Scatter(a.data(), matSize * matSize / nRanks, MPI_DOUBLE, sub_a.data(),
                matSize * matSize / nRanks, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(b.data(), b.size(), MPI_DOUBLE, 0, MPI_COMM_WORLD);
    matMult(sub_a.data(), b.data(), sub_c.data(), matSize / nRanks, matSize);
    MPI_Gather(sub_c.data(), matSize * matSize / nRanks, MPI_DOUBLE, c.data(),
               matSize * matSize / nRanks, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        std::string row;
        for (int i = 0; i < matSize; i++) {
            row.clear();
            for (int j = 0; j < matSize; j++) {
                row += std::to_string(c[i * matSize + j]);
                if (j != matSize - 1) {
                    row += ",";
                }
            }
            std::cout << row << std::endl;
        }
    }

    MPI_Finalize();

    return 0;
}
