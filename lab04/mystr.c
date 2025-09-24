#include <stdio.h>

int mystrlen(char *s){
	int len = 0;
	while(*s != '\0'){
		len++;
		*s++;
	}
	return len;
}

char *mystrcpy(char *dest, char *src){
	char *destStart = dest;
	while(*src != '\0'){
		*dest = *src;
		src++;
		dest++;
	}
	
	return destStart;
}

char *mystrcat(char *dest, char *src){
	char *destStart = dest;
	while(*dest != '\0'){
		dest++;
	}
	while(*src != '\0'){
		*dest = *src;
		src++;
		dest++;
	}

	return destStart;
}


void main()
{
   char *s1 = "Hello ";
   char *s2 = "CPSC 275!";
   char s3[20];

   printf("length of s1 = %d\n", mystrlen(s1));
   printf("s3 = %s\n", mystrcpy(s3, s1));
   printf("s3 = %s\n", mystrcat(s3, s2));
}
