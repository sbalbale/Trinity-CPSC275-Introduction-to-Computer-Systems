/**
* lab4ext1.c
* CPSC 275
* Sean Balbale
**/

/*

Write a short C program (lab4ex1.c) that declares and initializes
(to any value you like) a char, an int, a double, and a long.
Next declare and initialize a pointer to each of the four variables.
Your program should then print the address of each variable and the 
number of bytes each occupies in memory.
 
Use the %p formatting specifier to print the address in hexadecimal. 
Use the sizeof operator to determine the memory size allocated for
each variable. Your output should look like:

char: size = sizeof char, address = address of variable
int: size = sizeof int, address = address of variable
double: size = sizeof double, address = address of variable
long: size = sizeof long, address = address of variable
*/

#include <stdio.h>


int main(){
  int i = 0;
  char c = 'a';
  double d = 0.0;
  long l = 0L;

  printf("char: size = %zu, address = %p\n", sizeof(c), (void*)&c);
  printf("int: size = %zu, address = %p\n", sizeof(i), (void*)&i);
  printf("double: size = %zu, address = %p\n", sizeof(d), (void*)&d);
  printf("long: size = %zu, address = %p\n", sizeof(l), (void*)&l);

  return 0;

}
