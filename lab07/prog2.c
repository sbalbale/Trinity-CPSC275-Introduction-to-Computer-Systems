#include <stdio.h>

void func(int, int);

void main()
{
   int x, y;

   x = 5;
   y = 2;
   func(x, y);

   y = x - y;
   //x = 3;
   x = 4;
   func(x, y);
}

void func(int a, int b){
   printf("%d\n", a / (a - b));
}
