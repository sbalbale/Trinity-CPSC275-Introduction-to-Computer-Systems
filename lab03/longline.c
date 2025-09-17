/**
* dotprod.c 
* Sean Balbale
* CPSC 275 
**/

/**
* Download the C program, longline.c. Its overall purpose is to read 
* from the standard input and find and print the longest line (in characters).
* You should not change any of the skeleton code; your task is to implement the two 
* functions given as prototypes, namely, mygetline() and mycopy() and to write main().
**/
 
#include <stdio.h>
#define MAXLINE 100  // maximum input size of a single line

int mygetline(char line[]);
void mycopy(char to[],char from[]);
    
/* iterate through lines of input
   keep track of longest line seen to so far
   print longest line 
*/
void main(void){
   int len = 0;    		// current line length
   int max = 0;		// maximum length seen so far
   char line[MAXLINE];  //current input line
   char longest[MAXLINE]; // longest line is saved here

   /* YOUR CODE GOES HERE */
  while((len = mygetline(line)) != -1) {
    if(len > max) {
      max = len;
      mycopy(longest, line);
    }
  }
  printf("Longest line: %s \nLength: %d \n", longest, max);
} /* main */

/* getline: read a line into s, return length of line
   if EOF encountered, return -1
   (assumes last line has '\n')
   s should be a correctly formed string when complete ('\0')
*/
int mygetline(char s[]){
	int nChar = 0;
	char ch;

  ch = getchar();
  while(ch != EOF && nChar < MAXLINE - 1) {
    if(ch == '\n') {
      s[nChar] = ch;
      nChar++;
      break;
    }
    s[nChar] = ch;
    nChar++;
    ch = getchar();
  }
  return ch == EOF ? -1 : nChar;

} /* getline */

/* copy: copy  'from' into 'to' ;
   assumes 'from' is a correctly formed string 
*/
void mycopy(char to[], char from[]) {
  int i = 0;
  while((to[i] = from[i]) != '\0') {
    i++;
  }
} /* mycopy */
