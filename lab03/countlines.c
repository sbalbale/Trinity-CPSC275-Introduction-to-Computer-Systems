/**
* dotprod.c 
* Sean Balbale
* CPSC 275 
**/

/**
* Write a program named countlines.c consisting only of a main function 
* that counts the number of lines in a file using getchar().
**/

#include <stdio.h>

void main(){
  char c;
  int count=0;
  
  c = getchar();
  while(c != EOF){
    printf("%c", c);
    if(c == '\n'){
      count++;
    }
    c = getchar();
  }



  printf("Number of lines: %d\n", count);

}
