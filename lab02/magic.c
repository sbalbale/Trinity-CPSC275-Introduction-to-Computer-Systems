/**
* magic.c
* Sean Balbale
* CPSC 275
**/

/**
* Magic Square. A magic square of order n is an n × n array of distinct 
* integers such that all rows, all columns, and both diagonals sum to the 
* same value. Write a function named ismagic that, given a two-dimensional 
* array A and n as parameters, returns 1 if A is a magic square and 0 otherwise. 
* Using this function, write a C program named magic.c that reads the value of n
* followed by n rows of n integers, and prints whether A is a magic square. You 
* may assume that the maximum size of each dimension of the array is 100.
*
*
* Sample Input
* 3
* 8 1 6
* 3 5 7
* 4 9 2
*
* Sample Output
* It's a magic square!
*
* Sample Input
* 3
* 1 2 3
* 4 5 6
* 7 8 9
*
* Sample Output
* It's NOT a magic square!
**/

#include <stdio.h>

int isMagic(int n, int a[n][n]); // Updated function declaration

void main(){
  // Initialize variables and scan array size
  int n, i = 0, j = 0;
  printf("Enter the size of the magic square (n x n) (max 100): \n");
  scanf("%d", &n);

  // Scan array elements
  int a[n][n];
  printf("Enter the elements of the magic square: \n");
  for(i = 0; i < n; i++){
    for(j = 0; j < n; j++){
      scanf("%d", &a[i][j]);
    }
  }

  if(isMagic(n, a)){ // Pass n first
    printf("It's a magic square!\n");
  }
  else{
    printf("It's NOT a magic square!\n");
  }
}  

int isMagic(int n, int a[n][n]){ // Updated function definition
  int initSum, i, j;


  for(i = 0; i < n; i++){
    for(j = 0; j < n; j++){
      printf("%d ", a[i][j]);
    }
    printf("\n");
  }
  //Check horizontal sums  
  for(i = 0; i < n; i++){
    int sum = 0;
    for(j = 0; j < n; j++){
      sum += a[i][j];
    }
    printf("%d hor sum: %d\n", i,sum);
    if(i == 0){
      initSum = sum;
      printf("init sum: %d\n", initSum);
    }
    else if(sum != initSum){
      printf("init sum: %d, sum: %d\n", initSum, sum);
      return 0;
    }
  }

  //Check vertical sums
  for(j = 0; j < n; j++){
    int sum = 0;
    for(i = 0; i < n; i++){
      sum += a[i][j];
      printf("vert sum: %d\n", sum);
    }
    if(sum != initSum){
      return 0;
    }
  }

  //Check diagonal sums (top-left to bottom-right)
  int sum = 0;
  for(i = 0; i < n; i++){
    sum += a[i][i];
    printf("dia sum: %d\n", sum);
  }
  if(sum != initSum){
    return 0;
  }
  return 1;
}

