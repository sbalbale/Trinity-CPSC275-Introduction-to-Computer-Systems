#include <stdio.h>
#include <string.h>

char *strrev(char *, char *);

void main()
{
   char str[] = "hello";
   char out[20];

   printf("%s\n", strrev(out, str));
}

/* returns the reverse of src in dest */
char *strrev(char *dest, char *src) {
   char *p, *q;
   int len = strlen(src);
   int i;

   q = dest;
   for (i = len - 1; i >= 0; i--) {
      p = src + i;
      *q++ = *p;
   }
   *q = '\0';
   return dest;   
}
