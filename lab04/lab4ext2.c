#include <stdio.h>

void func1(int xval)
{
   int x;
   x = xval;
   /* print the address and value of x here */
}

void func2(int dummy)
{
   int y;
   /* print the address and value of y here */
}

void main()
{
   func1(7);
   func2(11);
}


