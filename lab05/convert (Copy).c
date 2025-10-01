/**
 * convert.c
 * Sean Balbale
 * CPSC 275
 **/


/**
 * Using recursion, write a C function:

void dec2bin(unsigned int n);

This function accepts a non-negative integer n as a parameter and prints its binary representation. 
You may not use the %b format specifier in printf; instead, construct the binary output manually. 
Using this function, write a C program named convert.c that reads a non-negative integer from standard input and prints its binary representation.

**/




#include <stdio.h>

void dec2bin(unsigned int n);

int power(int x, int y);

int main(){
	unsigned int n;

	printf("enter an unsigned integer: \n");
	scanf("%u \n",&n);
	printf("you entered %u \n",n);
	
	dec2bin(n);


	return 0;
}


void dec2bin(unsigned int n){
	unsigned int binary = 0;
	int maxBit = 0;
	int temp = n;
	if(temp%2 == 0){
		temp++;
	}
	while(temp > 0){
		temp = temp / 2;
		maxBit++;
	}
	maxBit--;
	printf("maxBit: %d \n",maxBit);
	while( n > 1 ){
		if(n%2==0){
			printf("%d \n",n%2);
		}
		else{
			binary = binary + 1;
		}
		printf("binary:%d n:%d maxBit:%d \n",binary,n,maxBit);
		binary = binary * 10;
		n = n - power(2,maxBit);
		maxBit--;
	}
	
	printf("binary:%d n:%d  \n",binary,n);
}

int power(int x,int y){
	if(y==0){return 1;}
	int total = x;
	while(y>1){
	total = total*x;
	y--;
	}
	return total;
}
