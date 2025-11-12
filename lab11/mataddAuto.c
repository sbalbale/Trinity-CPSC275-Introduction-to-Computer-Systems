#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void initmat(float *, int);
void madd(float *, float *, float *, int);

int main()
{
   float *A, *B, *C;
   int N[] = {512,1024,2048,4096};
   clock_t start, end;
   double cpu_time_used;


   for(int idx=0; idx<4; idx++) {
       // Allocate three N-by-N matrices in the heap
       A = (float *) malloc(N[idx] * N[idx] * sizeof(float));
       B = (float *) malloc(N[idx] * N[idx] * sizeof(float));
       C = (float *) malloc(N[idx] * N[idx] * sizeof(float));

       if (A == NULL || B == NULL || C == NULL) {
          printf("Memory allocation failed\n");
          return 1;
       }

       // Initialize matrices A and B
       initmat(A, N[idx]);
       initmat(B, N[idx]);

       // Compute C = A + B
       start = clock();
       madd(A, B, C, N[idx]);
       end = clock();

       cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
       printf("Matrix addition (N=%d) took %f seconds\n", N[idx], cpu_time_used);

       // Free the allocated space
       free(A);
       free(B);
       free(C);
   }

   return 0;
}

void madd(float *A, float *B, float *C, int N)
{
   int i, j;

   for (i = 0; i < N; i++) {
      for (j = 0; j < N; j++) {
         C[i*N+j] = A[i*N+j] + B[i*N+j];
      }
   }
}

void initmat(float *mat, int N)
{
   int i, j;

   for (i = 0; i < N; i++)
      for (j = 0; j < N; j++)
         mat[i*N+j] = i + j;
}
