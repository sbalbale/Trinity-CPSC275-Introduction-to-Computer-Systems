#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void initmat(float *, int);
void madd(float *, float *, float *, int);

int main(int argc, char *argv[])
{
   float *A, *B, *C;
   int N;

   if (argc != 2) {
      printf("Usage: %s N\n", argv[0]);
      return 1;
   }

   N = atoi(argv[1]);

   // Allocate three N-by-N matrices in the heap
   A = (float *) malloc(N * N * sizeof(float));
   B = (float *) malloc(N * N * sizeof(float));
   C = (float *) malloc(N * N * sizeof(float));

   if (A == NULL || B == NULL || C == NULL) {
      printf("Memory allocation failed\n");
      return 1;
   }

   // Initialize matrices A and B
   initmat(A, N);
   initmat(B, N);

   // Compute C = A + B
   madd(A, B, C, N);

   // Free the allocated space
   free(A);
   free(B);
   free(C);

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
