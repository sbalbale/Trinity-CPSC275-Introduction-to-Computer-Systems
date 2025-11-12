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

int main()
{
    float *A, *B, *C;
    int N[] = {128, 256, 512, 1024, 2048};
    clock_t start, end;
    double cpu_time_used;

    for (int idx = 0; idx < 5; idx++)
    {
        // Allocate three N-by-N matrices in the heap
        A = (float *)malloc(N[idx] * N[idx] * sizeof(float));
        B = (float *)malloc(N[idx] * N[idx] * sizeof(float));
        C = (float *)malloc(N[idx] * N[idx] * sizeof(float));

        if (A == NULL || B == NULL || C == NULL)
        {
            printf("Memory allocation failed\n");
            return 1;
        }

        // Initialize matrices A and B
        initmat(A, N[idx]);
        initmat(B, N[idx]);

        // Compute C = A + B
        start = clock();
        mmul_ijk(A, B, C, N[idx]);
        end = clock();

        cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
        printf("Matrix multiplication (N=%d) took %f seconds\n", N[idx], cpu_time_used);

        // Free the allocated space
        free(A);
        free(B);
        free(C);
    }

    printf("Comparing diffent loop orderings with N = 2048\n");
    A = (float *)malloc(2048 * 2048 * sizeof(float));
    B = (float *)malloc(2048 * 2048 * sizeof(float));
    C = (float *)malloc(2048 * 2048 * sizeof(float));
    initmat(A, 2048);
    initmat(B, 2048);
    

    // ijk
    initmat(C, 2048);
    start = clock();
    mmul_ijk(A, B, C, 2048);
    end = clock();
    printf("Matrix multiplication (ijk) took %f seconds\n", ((double)(end - start)) / CLOCKS_PER_SEC);

    // ikj
    initmat(C, 2048);
    start = clock();
    mmul_ikj(A, B, C, 2048);
    end = clock();
    printf("Matrix multiplication (ikj) took %f seconds\n", ((double)(end - start)) / CLOCKS_PER_SEC);
    
    // jik
    initmat(C, 2048);
    start = clock();
    mmul_jik(A, B, C, 2048);
    end = clock();
    printf("Matrix multiplication (jik) took %f seconds\n", ((double)(end - start)) / CLOCKS_PER_SEC);
    
    // jki
    initmat(C, 2048);
    start = clock();
    mmul_jki(A, B, C, 2048);
    end = clock();
    printf("Matrix multiplication (jki) took %f seconds\n", ((double)(end - start)) / CLOCKS_PER_SEC);
   
    // kij
    initmat(C, 2048);
    start = clock();
    mmul_kij(A, B, C, 2048);
    end = clock();
    printf("Matrix multiplication (kij) took %f seconds\n", ((double)(end - start)) / CLOCKS_PER_SEC);
    
    // kji
    initmat(C, 2048);
    start = clock();
    mmul_kji(A, B, C, 2048);
    end = clock();
    printf("Matrix multiplication (kji) took %f seconds\n", ((double)(end - start)) / CLOCKS_PER_SEC);



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
