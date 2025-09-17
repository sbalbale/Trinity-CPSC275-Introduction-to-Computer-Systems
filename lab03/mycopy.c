/**
* Write a program named mycopy.c consisting only of a main() function 
* that uses getchar() and putchar() to copy one character at a time from 
* the input stream to the output stream. Note that end-of-file is defined
* as EOF in stdio.h. In Linux, end-of-file is marked by Ctrl-D from the keyboard.
* Test your program with keyboard input and output.
**/

#include <stdio.h>


int main() {
    int ch;

    while ((ch = getchar()) != EOF) {
        putchar(ch);
    }

    return 0;
}

