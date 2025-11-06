/**
* File: sort.c
* Purpose: read n integers into an array and sort them using insertion sort
* Author: Sean Balbale
* Date: 11/11/2025
**/

#include <stdio.h>

#define MAX_SIZE 1000

extern void insertion_sort(int *arr, int n);

int main() {
    int arr[MAX_SIZE];
    // Read the number of integers
    printf("Enter an array of integers (max %d): ", MAX_SIZE);
    int n = 0;
    while (n < MAX_SIZE && scanf("%d", &arr[n]) == 1) {
        n++;
    }
    

    // Sort the array using insertion sort
    insertion_sort(arr, n);

    // Print the sorted array
    printf("\nSorted integers:\n");
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    return 0;
}