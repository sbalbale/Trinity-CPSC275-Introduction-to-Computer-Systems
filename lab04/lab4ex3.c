#include <stdio.h>

void main( )
{
   char c = 'Z';
   char *cp = &c;

   printf("cp is %p\n", cp);
   printf("The character at cp is %c\n", *cp);

   /* Pointer arithmetic - see what cp+1 is */
   cp = cp + 1;
   printf("cp is now %p\n", cp);

   /* Note: Do not try to print *cp here, because it points to the
      memory location unallocated to your program */

   int i = 0;
   int *ip = &i;

   printf("ip is: %p\n", ip);
   printf("The character at ip is %i\n", *ip);

   ip = ip + 1;
   printf("ip is now %p\n", ip);
}
