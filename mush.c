#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <limits.h>

int length = 2048;
int max_args = 128;

/* Handle and execute commands using child processes */
void handleCommand(char *args[]) {
    // Fork a new process
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // Child process
        // Execute the command
        if (execvp(args[0], args) == -1) {
            perror("execvp failed");
            exit(EXIT_FAILURE);
        }
    } else {
        // Parent process
        // Wait for the child process to terminate
        wait(NULL);
    }
}

/* Print the shell prompt from current working directory */
void printPrompt() {
    char *cwd; // Current working directory
        
    // allocate memory for cwd
    if ((cwd = malloc(PATH_MAX)) == NULL) {
    perror("malloc failed");
    exit(EXIT_FAILURE);
    }
    // check if getcwd failed
    if (getcwd(cwd, PATH_MAX) == NULL) {
        perror("getcwd failed");
        exit(EXIT_FAILURE);
    }
    printf("mush:%s> ", cwd); // Print the shell prompt
    free(cwd); // Free the allocated memory
}

int main() {
    char command[length];
    char *args[max_args];
    char *token;

    while (1) {
        
        printPrompt(); // Print the shell prompt

        // Read the command line
        if (fgets(command, length, stdin) == NULL) {
            perror("fgets failed");
            exit(EXIT_FAILURE);
        }

        // Tokenize the command line using if-do-while
        int i = 0;
        
        if ((token = strtok(command, " \n")) != NULL) {
            do {
                args[i] = token;
                i++;
            } while ((token = strtok(NULL, " \n")) != NULL);
          }
        
        args[i] = NULL; // Set the last element of the args array to NULL

        // Handle cd command
        if (strcmp(args[0], "cd") == 0) {
            if (chdir(args[1]) == -1) {
                perror("chdir failed");
            } 
            continue; // Skip forking a child process for cd command
        }

        // Handle exit command
        if (strcmp(args[0], "exit") == 0) {
            exit(0);
        }

        // Handle other commands
        handleCommand(args);
      }

    return 0;
}
