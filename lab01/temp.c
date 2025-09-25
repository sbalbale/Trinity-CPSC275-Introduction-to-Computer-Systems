/**
 * temp.c
 * Sean Balbale
 * CPSC 275
 **/


#include <stdio.h>
void main() {
	int tempf, tempc;
	printf("Enter a number in Fahrenheit \n");
	scanf("%d",&tempf);
	tempc = (tempf-32)*(5.0/9.0);
	printf("%d degrees in Fahrenheit is %d degrees in Celcius.\n",tempf,tempc);
}
