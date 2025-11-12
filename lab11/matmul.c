#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void initmat(float *, int);
void mmul_ijk(float *A, float *B, float *C, int N);
void mmul_ikj(float *A, float *B, float *C, int N);
void mmul_jik(float *A, float *B, float *C, int N);
void mmul_jki(float *A, float *B, float *C, int N);
void mmul_kij(float *A, float *B, float *C, int N);
void mmul_kji(float *A, float *B, float *C, int N);

int main(int argc, char *argv[])
{
    float *A, *B, *C;
    int N;

    if (argc != 2)
    {
        printf("Usage: %s N\n", argv[0]);
        return 1;
    }

    N = atoi(argv[1]);

    // Allocate three N-by-N matrices in the heap
    A = (float *)malloc(N * N * sizeof(float));
    B = (float *)malloc(N * N * sizeof(float));
    C = (float *)malloc(N * N * sizeof(float));

    if (A == NULL || B == NULL || C == NULL)
    {
        printf("Memory allocation failed\n");
        return 1;
    }

    // Initialize matrices A and B
    initmat(A, N);
    initmat(B, N);

    // Compute C = A + B
    mmul_ijk(A, B, C, N);

    // Free the allocated space
    free(A);
    free(B);
    free(C);

    return 0;
}

void initmat(float *mat, int N)
{
    int i, j;

    for (i = 0; i < N; i++)
        for (j = 0; j < N; j++)
            mat[i * N + j] = i + j;
}

// version ijk
// Locality: A has good spatial locality (row-wise access in innermost loop)
//           B has poor spatial locality (column-wise access, stride N)
//           C has good temporal locality (reused in register as sum)
// Overall: Moderate performance due to poor B access pattern
void mmul_ijk(float *A, float *B, float *C, int N)
{
    int i, j, k;
    float sum;
    for (i = 0; i < N; i++)
        for (j = 0; j < N; j++)
        {
            sum = 0.0;
            for (k = 0; k < N; k++)
                sum += A[i * N + k] * B[k * N + j];
            C[i * N + j] = sum;
        }
}

// version ikj
// Locality: A has good temporal locality (loaded once per middle loop)
//           B has excellent spatial locality (row-wise access in innermost loop)
//           C has excellent spatial locality (row-wise access in innermost loop)
// Overall: Best performance - both B and C accessed with stride 1
void mmul_ikj(float *A, float *B, float *C, int N)
{
    int i, j, k;
    float r;
    for (i = 0; i < N; i++)
        for (k = 0; k < N; k++)
        {
            r = A[i * N + k];
            for (j = 0; j < N; j++)
                C[i * N + j] += r * B[k * N + j];
        }
}

// version jik
// Locality: A has good spatial locality (row-wise access in innermost loop)
//           B has poor spatial locality (column-wise access, stride N)
//           C has good temporal locality (reused in register as sum)
// Overall: Moderate performance, similar to ijk due to poor B access pattern
void mmul_jik(float *A, float *B, float *C, int N)
{
    int i, j, k;
    float sum;
    for (j = 0; j < N; j++)
        for (i = 0; i < N; i++)
        {
            sum = 0.0;
            for (k = 0; k < N; k++)
                sum += A[i * N + k] * B[k * N + j];
            C[i * N + j] = sum;
        }
}

// version jki
// Locality: A has poor spatial locality (column-wise access, stride N)
//           B has good temporal locality (loaded once per middle loop)
//           C has poor spatial locality (column-wise access, stride N)
// Overall: Worst performance - both A and C accessed with stride N
void mmul_jki(float *A, float *B, float *C, int N)
{
    int i, j, k;
    float r;
    for (j = 0; j < N; j++)
        for (k = 0; k < N; k++)
        {
            r = B[k * N + j];
            for (i = 0; i < N; i++)
                C[i * N + j] += A[i * N + k] * r;
        }
}

// version kij
// Locality: A has good temporal locality (loaded once per middle loop)
//           B has excellent spatial locality (row-wise access in innermost loop)
//           C has excellent spatial locality (row-wise access in innermost loop)
// Overall: Best performance - both B and C accessed with stride 1, same as ikj
void mmul_kij(float *A, float *B, float *C, int N)
{
    int i, j, k;
    float r;
    for (k = 0; k < N; k++)
        for (i = 0; i < N; i++)
        {
            r = A[i * N + k];
            for (j = 0; j < N; j++)
                C[i * N + j] += r * B[k * N + j];
        }
}

// version kji
// Locality: A has poor spatial locality (column-wise access, stride N)
//           B has good temporal locality (loaded once per middle loop)
//           C has poor spatial locality (column-wise access, stride N)
// Overall: Worst performance - both A and C accessed with stride N, same as jki
void mmul_kji(float *A, float *B, float *C, int N)
{
    int i, j, k;
    float r;
    for (k = 0; k < N; k++)
        for (j = 0; j < N; j++)
        {
            r = B[k * N + j];
            for (i = 0; i < N; i++)
                C[i * N + j] += A[i * N + k] * r;
        }
}
