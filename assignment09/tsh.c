/**
* File: tsh.c
* Purpose: A simple shell program that reads user input, parses commands,
*          executes them, and handles built-in commands like "quit".
* Author: Sean Balbale
* Date: 12/8/2025
**/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_LINE 1024
#define MAX_ARGS 128



/**
 * Function: main
 * Description: A simple shell program that reads user input, parses commands,
 *             executes them, and handles built-in commands like "quit".
 * Returns: 0 on successful execution.
 * 
 */
int main() {
    char cmd_line[MAX_LINE];
    char *args[MAX_ARGS];
    char *token;
    pid_t pid;
    int status;

    while (1) {
        // Display prompt
        printf("tsh> ");
        fflush(stdout);

        // Read command line
        if (fgets(cmd_line, MAX_LINE, stdin) == NULL) {
            // End of file (Ctrl+D)
            printf("\n");
            break;
        }

        // Remove trailing newline
        if (cmd_line[strlen(cmd_line) - 1] == '\n') {
            cmd_line[strlen(cmd_line) - 1] = '\0';
        }

        // Parse command line into tokens
        int nargs = 0;                                      // Argument count
        token = strtok(cmd_line, " \t\n");                  // Split by whitespace
        while (token != NULL && nargs < MAX_ARGS - 1) {     // Leave space for NULL terminator
            args[nargs++] = token;                          // Store token in args array
            token = strtok(NULL, " \t\n");                  // Get next token    
        }
        args[nargs] = NULL; // Null-terminate the argument list

        // If no command entered, continue
        if (nargs == 0) {
            continue;
        }

        // Check for built-in command "quit"
        if (strcmp(args[0], "quit") == 0) {
            exit(0);
        }

        // Fork a child process
        pid = fork();

        if (pid < 0) {
            // Fork failed
            perror("fork");
            continue;
        } else if (pid == 0) {
            // Child process
            if (execvp(args[0], args) < 0) { // Execute the command (args[0]) with arguments (args)
                // execvp only returns if it fails
                perror(args[0]); // Print error message like "ls: No such file or directory"
                exit(1);
            }
        } else {
            // Parent process
            if (wait(&status) < 0) {
                perror("wait");
            }
        }
    }

    return 0;
}
