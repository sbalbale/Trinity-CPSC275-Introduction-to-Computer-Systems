#include <stdio.h>

int sum(int *, int);

void main(void) 
{
	   int size = 5;
	      int arr[] = {10, 20, 30, 40, 50};
	         int result;

		    result = sum(arr, size);
		       printf("sum = %d\n", result);
}

int sum(int a[], int n) {
	   int i, total = 0;
	      for (i = 0; i < n; i++)
		            total += a[i];
	         return total;
}
