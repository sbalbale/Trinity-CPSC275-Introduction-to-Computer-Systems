#include <stdio.h>

void func(int *, int);

void main()
{
   int x[] = {1, 2, 3, 4, 5};
   //int y = 10000;
  int y = 3;
  
   func(x, y);
}

void func(int a[], int p){
   printf("%d\n", a[p]);
}
