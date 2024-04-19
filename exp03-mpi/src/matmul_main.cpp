#include <mpi.h>
#include <iostream>
#include <vector>

#define N 4  // 假设矩阵大小为 N x N

void matMult(double *a, double *b, double *c, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            c[i * n + j] = 0;
            for (int k = 0; k < n; k++) {
                c[i * n + j] += a[i * n + k] * b[k * n + j];
            }
        }
    }
}

int main(int argc, char **argv) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int n = N;
    double *a, *b, *c, *sub_a, *sub_c;

    a = (double *)malloc(n * n * sizeof(double));
    b = (double *)malloc(n * n * sizeof(double));
    c = (double *)malloc(n * n * sizeof(double));
    sub_a = (double *)malloc(n * n / size * sizeof(double));
    sub_c = (double *)malloc(n * n / size * sizeof(double));

    // std::vector<double> a(n*n);
    // std::vector<double> b(n*n);
    // std::vector<double> c(n*n);

    if (rank == 0) {
        for (int i = 0; i < n * n; i++) {
            a[i] = 1.0;
            b[i] = 1.0;
        }
    }

    MPI_Bcast(b, n * n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Scatter(a, n * n / size, MPI_DOUBLE, sub_a, n * n / size, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    matMult(sub_a, b, sub_c, n / size);
    MPI_Gather(sub_c, n * n / size, MPI_DOUBLE, c, n * n / size, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    if (rank == 0) {
        printf("Result Matrix C:\n");
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                printf("%6.2f ", c[i * n + j]);
            }
            printf("\n");
        }
    }
    free(a);
    free(b);
    free(c);
    free(sub_a);
    free(sub_c);
    MPI_Finalize();
    return 0;
}
