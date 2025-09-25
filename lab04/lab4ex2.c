#include <stdio.h>

void func1(int xval)
{
   int x;
   x = xval;
   /* print the address and value of x here */
   printf("x) value:%i , address: %p \n",x,(void*)&x);
}

void func2(int dummy)
{
   int y;
   /* print the address and value of y here */
   printf("y) value:%i, address: %p \n",y,(void*)&y);
}

void main()
{
   func1(7);
   func2(11);
}


