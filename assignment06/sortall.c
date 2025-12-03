/**
* File: sortall.c
* Purpose: read n integers into an array and sort them using insertion, bubble, and selection sort
* Author: Sean Balbale
* Date: 12/03/2025
**/

#include <stdio.h>
#include <string.h>

#define MAX_SIZE 1000

extern void insertion_sort(int *arr, int n);
extern void bubble_sort(int *arr, int n);
extern void selection_sort(int *arr, int n);

void print_array(int *arr, int n) {
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int main() {
    int arr[MAX_SIZE];
    int arr_copy[MAX_SIZE];
    
    // Read the number of integers
    printf("Enter an array of integers (max %d): ", MAX_SIZE);
    int n = 0;
    while (n < MAX_SIZE && scanf("%d", &arr[n]) == 1) {
        n++;
    }

    // 1. Insertion Sort
    memcpy(arr_copy, arr, n * sizeof(int));
    insertion_sort(arr_copy, n);
    printf("\nInsertion Sort:\n");
    print_array(arr_copy, n);

    // 2. Bubble Sort
    memcpy(arr_copy, arr, n * sizeof(int));
    bubble_sort(arr_copy, n);
    printf("\nBubble Sort:\n");
    print_array(arr_copy, n);

    // 3. Selection Sort
    memcpy(arr_copy, arr, n * sizeof(int));
    selection_sort(arr_copy, n);
    printf("\nSelection Sort:\n");
    print_array(arr_copy, n);

    return 0;
}
