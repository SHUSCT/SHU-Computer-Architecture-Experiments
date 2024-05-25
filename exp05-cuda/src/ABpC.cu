#include <cstdint>
#include <cuda_runtime.h>

extern "C"
{
    void cudaABpC(float* A, float* B, float* C, size_t a, size_t b, size_t c);
}

/**
 * @brief Matrix multiplication: C = A * B.
 *
 * @param A Matrix of shape (a, b).
 * @param B Matrix of shape (b, c).
 * @param C Matrix of shape (a, c).
 */
__global__ void cudaABpC_kernel(float* A, float* B, float* C, size_t a, size_t b, size_t c)
{
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    int j = blockIdx.y * blockDim.y + threadIdx.y;

    if (i < a && j < c) {
        float sum = 0.0f;
        for (int k = 0; k < b; k++) {
            sum += A[i * b + k] * B[k * c + j];
        }
        C[i * c + j] = sum;
    }
}

void cudaABpC(float* A, float* B, float* C, size_t a, size_t b, size_t c)
{
    float* d_A;
    float* d_B;
    float* d_C;

    cudaMalloc(&d_A, a * b * sizeof(float));
    cudaMalloc(&d_B, b * c * sizeof(float));
    cudaMalloc(&d_C, a * c * sizeof(float));

    cudaMemcpy(d_A, A, a * b * sizeof(float), cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, B, b * c * sizeof(float), cudaMemcpyHostToDevice);

    dim3 block(16, 16);
    dim3 grid((a + block.x - 1) / block.x, (c + block.y - 1) / block.y);

    cudaABpC_kernel<<<grid, block>>>(d_A, d_B, d_C, a, b, c);

    cudaMemcpy(C, d_C, a * c * sizeof(float), cudaMemcpyDeviceToHost);

    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_C);
}