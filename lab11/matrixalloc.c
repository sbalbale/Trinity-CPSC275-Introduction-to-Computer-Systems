#include <stdio.h>
#include <stdlib.h> 

void initmat(float *, int);

void main(int argc, char *argv[])
{
   float *A;
   int N = atoi(argv[1]);

   // Allocate N-by-N matrix in the heap.
   A = (float *) malloc(N * N * sizeof(float));

   // Initialize the matrix.
   initmat(A, N);

   // Free the allocated space
   free(A);
}

void initmat(float *mat, int N)
{
   int i, j;

   for (i = 0; i < N; i++)
      for (j = 0; j < N; j++)
   	 mat[i*N+j] = i + j;
}
