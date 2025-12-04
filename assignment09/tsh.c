#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_LINE 1024
#define MAX_ARGS 128

int main() {
    char cmd_line[MAX_LINE];
    char *argv[MAX_ARGS];
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
        int argc = 0;                                       // Argument count
        token = strtok(cmd_line, " ");                      // Split by spaces
        while (token != NULL && argc < MAX_ARGS - 1) {      // Leave space for NULL terminator
            argv[argc++] = token;                           // Store token in argv array
            token = strtok(NULL, " ");                      // Get next token    
        }
        argv[argc] = NULL; // Null-terminate the argument list

        // If no command entered, continue
        if (argc == 0) {
            continue;
        }

        // Check for built-in command "quit"
        if (strcmp(argv[0], "quit") == 0) {
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
            if (execvp(argv[0], argv) < 0) { // Execute the command (argv[0]) with arguments (argv)
                // execvp only returns if it fails
                perror(argv[0]); // Print error message like "ls: No such file or directory"
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
