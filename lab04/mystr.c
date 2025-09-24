#include <stdio.h>

void main()
{
   char *s1 = "Hello ";
   char *s2 = "CPSC 275!";
   char s3[20];

   printf("length of s1 = %d\n", mystrlen(s1));
   printf("s3 = %s\n", mystrcpy(s3, s1));
   printf("s3 = %s\n", mystrcat(s3, s2));
}
