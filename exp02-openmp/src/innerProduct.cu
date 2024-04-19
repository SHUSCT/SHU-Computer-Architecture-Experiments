#include "InnerProduct.hpp"
#include <cstdio>

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

    cudaError_t err;
    err = cudaMalloc(&d_a, n * sizeof(float));
    if (err != cudaSuccess) {
        printf("[CUDA ERROR]: %s\n", cudaGetErrorString(err));
        return;
    }
    err = cudaMalloc(&d_b, n * sizeof(float));
    if (err != cudaSuccess) {
        printf("[CUDA ERROR]: %s\n", cudaGetErrorString(err));
        return;
    }
    err = cudaMalloc(&d_c, n * sizeof(float));
    if (err != cudaSuccess) {
        printf("[CUDA ERROR]: %s\n", cudaGetErrorString(err));
        return;
    }

    err = cudaMemcpy(d_a, a, n * sizeof(float), cudaMemcpyHostToDevice);
    if (err != cudaSuccess) {
        printf("[CUDA ERROR]: %s\n", cudaGetErrorString(err));
        return;
    }
    err = cudaMemcpy(d_b, b, n * sizeof(float), cudaMemcpyHostToDevice);
    if (err != cudaSuccess) {
        printf("[CUDA ERROR]: %s\n", cudaGetErrorString(err));
        return;
    }

    int blockSize = 512;
    int numBlocks = (n + blockSize - 1) / blockSize;

    cuInnerProductKernel<<<numBlocks, blockSize>>>(d_a, d_b, d_c, n);

    err = cudaMemcpy(c, d_c, n * sizeof(float), cudaMemcpyDeviceToHost);
    if (err != cudaSuccess) {
        printf("[CUDA ERROR]: %s\n", cudaGetErrorString(err));
        return;
    }

    err = cudaFree(d_a);
    if (err != cudaSuccess) {
        printf("[CUDA ERROR]: %s\n", cudaGetErrorString(err));
        return;
    }
    err = cudaFree(d_b);
    if (err != cudaSuccess) {
        printf("[CUDA ERROR]: %s\n", cudaGetErrorString(err));
        return;
    }
    err = cudaFree(d_c);
    if (err != cudaSuccess) {
        printf("[CUDA ERROR]: %s\n", cudaGetErrorString(err));
        return;
    }
}