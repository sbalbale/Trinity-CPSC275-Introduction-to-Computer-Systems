/**
* File: reverse.c
* Purpose: read n integers into an array and reverse them in place
* Author: Sean Balbale
* Date: 12/3/2025
**/

#include <stdio.h>

#define MAX_SIZE 1000

extern void reverse(int *arr, int n);

int main() {
    int arr[MAX_SIZE];
    // Read the number of integers
    int n = 0;
    while (n < MAX_SIZE && scanf("%d", &arr[n]) == 1) {
        n++;
    }    

    // Sort the array using insertion sort
    reverse(arr, n);

    // Print the sorted array
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    return 0;
}