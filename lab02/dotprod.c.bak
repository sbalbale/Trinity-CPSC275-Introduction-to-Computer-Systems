/**
* dotprod.c 
* Sean Balbale
* CPSC 275 
**/

/**
* Write a C function named dotprod which accepts, as parameters, a positive integer n and two vectors, x and y of length n, and returns their dot product:
*
*   x · y = x[0]*y[0] + ··· + x[n-1]*y[n-1]
*
* You may assume that the maximum size of both arrays is 100. 
* Using this function, write a C program named dotprod.c which reads the values of 
* n, x, and y, and prints the dot product of x and y.
*
* Sample Input
* 10
* 1.0 2.0 3.0 4.0 5.0 6.0 7.0 8.0 9.0 10.0
* 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0
**/

#include <stdio.h>

float dotproduct(int n, float x[], float y[]);

void main(){
//Find array length
  int n, i=0;
  printf("Enter the size of the vectors (max 100):\n");
  scanf("%d", &n);

//Find array elements
  float x[n], y[n], dotprod;
  printf("Enter the elements of vector x:\n");
  while(i < n){
    scanf("%f", &x[i]);
    i++;
  }
  i = 0;
  printf("Enter the elements of vector y:\n");
  while(i < n){
    scanf("%f", &y[i]);
    i++;
  }
  i = 0;
/**
//Calculate dot product
  while(i < n){
//    printf("x[%d] = %f, y[%d] = %f\n", i, x[i], i, y[i]);  
    dotprod += x[i] * y[i];
    i++;
  }
  **/
  dotprod = dotproduct(n, x, y);
  printf("The dot product is: %.1f\n", dotprod);
}

float dotproduct(int n, float x[], float y[]){
  int i = 0;
  float dotprod = 0;
  while(i < n){
    dotprod += x[i] * y[i];
    i++;
  }
  return dotprod;
}
