#include <iostream>
#include <cuda_runtime.h>
__global__ void vectorAdd(float *A, float *B, float *C, int N) {
    int idx = threadIdx.x + blockIdx.x * blockDim.x;
    if (idx < N) {
        C[idx] = A[idx] + B[idx];
    }
}
int main() {
    int N = 10; 
    size_t size = N * sizeof(float);
    float *A, *B, *C, *d_A, *d_B, *d_C;
    A = (float*)malloc(size);
    B = (float*)malloc(size);
    C = (float*)malloc(size);
    cudaMalloc(&d_A, size);
    cudaMalloc(&d_B, size);
    cudaMalloc(&d_C, size);
    for (int i = 0; i < N; i++) {
        A[i] = i + 1;
        B[i] = (i + 1) * 2;
    }
    cudaMemcpy(d_A, A, size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, B, size, cudaMemcpyHostToDevice);
    int threadsPerBlock = 256;
    int blocksPerGrid = (N + threadsPerBlock - 1) / threadsPerBlock;
    vectorAdd<<<blocksPerGrid, threadsPerBlock>>>(d_A, d_B, d_C, N);
    cudaMemcpy(C, d_C, size, cudaMemcpyDeviceToHost);
    std::cout << "Vector A: ";
    for (int i = 0; i < N; i++) {
        std::cout << A[i] << " ";
    }
    std::cout << std::endl;
    std::cout << "Vector B: ";
    for (int i = 0; i < N; i++) {
        std::cout << B[i] << " ";
    }
    std::cout << std::endl;
    std::cout << "Calculations (A[i] + B[i]):" << std::endl;
    for (int i = 0; i < N; i++) {
        std::cout << "C[" << i << "] = " << A[i] << " + " << B[i] << " = " << C[i] << std::endl;
    }
    free(A);
    free(B);
    free(C);
    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_C);
    return 0;
}
