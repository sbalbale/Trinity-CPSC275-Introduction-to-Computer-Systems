/**
 * digits.c
 * Sean Balbale
 * CPSC 275
 **/


#include <stdio.h>

int main() {
    int num, digit, temp, reverse = 0;
    
    scanf("%d", &num);
    
    if (num == 0) {
        printf("0\n");
        return 0;
    }

    // Reverse the number
    temp = num;
    while (temp > 0) {
        reverse = reverse * 10 + (temp % 10);
        temp /= 10;
    }
    
    // Print each digit of the reversed number with an asterisk
    while (reverse > 0) {
        digit = reverse % 10;
        printf("%d", digit);
        reverse /= 10;
        if (reverse > 0) {
            printf("*");
        }
    }
    printf("\n");
    
    return 0;
}
