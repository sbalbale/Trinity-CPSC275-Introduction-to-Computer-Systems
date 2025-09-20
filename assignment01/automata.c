/**
 * automata.c
 * Sean Balbale
 * CPC 275
 **/

/**
 * A one-dimensional cellular automaton takes a string consisting of the characters 0 and 1 and
 * updates it according to predetermined rules. Each rule examines three consecutive 
 * characters—the character at position k and its two neighbors—and determines the new value 
 * at position k in the next string.
 **/

#include <stdio.h>

#define MAX_LEN 100

void cellular_automata(char s[], int pattern, int n);

int main(){
    int m; // number of test cases
    scanf("%d", &m); // number of test cases
    for (int i = 0; i < m; i++) {
        char s[MAX_LEN]; // initial string
        int pattern; // pattern number
        int generations; // number of generations
        scanf("%s %d %d", s, &pattern, &generations);
        // printf("Test case %d: Initial: %s, Pattern: %d, Generations: %d\n", i+1, s, pattern, generations);
        cellular_automata(s, pattern, generations);
    }
}

void cellular_automata(char s[], int pattern, int n) {
    int len = 0;
    while (s[len] != '\0') len++; // calculate length of s
    // printgf("Length: %d\n", len);

    // Create a rules array based on the pattern number
    int rules[8];
    for (int i = 0; i < 8; i++) {
        rules[i] = (pattern >> i) & 1; // extract each bit of the pattern
    }

    char current[MAX_LEN];
    char next[MAX_LEN];
    
    // Initialize current gen
    for (int i = 0; i < len; i++) {
        current[i] = s[i];
    }
    current[len] = '\0';
    // printf("Initial: %s\n", current);

    // Apply the rules n times
    for (int gen = 0; gen < n; gen++) {
        for (int i = 0; i < len; i++) {
            // Get the left, center, and right neighbors with circular boundary conditions
            char left = current[(i - 1 + len) % len];
            char center = current[i];
            char right = current[(i + 1) % len];
            // printf("Gen %d, Pos %d: L=%c C=%c R=%c\n", gen, i, left, center, right);

            // Determine the index for the rules array
            int index = (left - '0') * 4 + (center - '0') * 2 + (right - '0');
            next[i] = rules[index] + '0'; // apply the rule
        }
        next[len] = '\0';
        // printf("After Gen %d: %s\n", gen + 1, next);

        // Copy next generation to current for the next iteration
        for (int i = 0; i < len; i++) {
            current[i] = next[i];
        }
        current[len] = '\0';
    }

    // Print final result
    printf("%s\n", current);
}


