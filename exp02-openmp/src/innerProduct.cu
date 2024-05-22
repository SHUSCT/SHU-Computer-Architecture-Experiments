#include <cuda_runtime.h>
#include <cuda.h>
#include <cstdio>

extern "C"
{
    void cuInnerProduct(float* a, float* b, float* c, int n);
}

__global__ void cuInnerProductKernel(float* a, float* b, float* c, int n)
{
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    if (i < n) {
        c[i] = a[i] * b[i];
    }
}

void cuInnerProduct(float* a, float* b, float* c, int n)
{
    float *d_a, *d_b, *d_c;

    cudaMalloc(&d_a, n * sizeof(float));
    cudaMalloc(&d_b, n * sizeof(float));
    cudaMalloc(&d_c, n * sizeof(float));
    cudaMemcpy(d_a, a, n * sizeof(float), cudaMemcpyHostToDevice);
    cudaMemcpy(d_b, b, n * sizeof(float), cudaMemcpyHostToDevice);

    int blockSize = 512;
    int numBlocks = (n + blockSize - 1) / blockSize;

    cuInnerProductKernel<<<numBlocks, blockSize>>>(d_a, d_b, d_c, n);

    cudaMemcpy(c, d_c, n * sizeof(float), cudaMemcpyDeviceToHost);
    cudaFree(d_a);
    cudaFree(d_b);
    cudaFree(d_c);
}