#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_INPUT 1024
#define MAX_ARGS 64

int main() {
    char input[MAX_INPUT];
    char *args[MAX_ARGS];

    while (1) {
        printf("myshell> ");
        fflush(stdout);

        if (!fgets(input, MAX_INPUT, stdin)) {
            break;
        }

        input[strcspn(input, "\n")] = 0;

        if (strlen(input) == 0) {
            continue;
        }

        int i=0;
	char *token = strtok(input, " ");

	while (token!= NULL && i < MAX_ARGS -1) {
	    args[i++] = token;
	    token = strtok(NULL, " ");
	} 
	// loops over all words then stores pointers in args[]
	args[i] = NULL;
	
	if (strcmp(args[0], "exit") == 0) {
	    break;
	}

	if (strcmp(args[0], "cd") == 0) {
	    if (args[1] == NULL) {
                fprintf(stderr, "myshell: expected argument to \"cd\"\n");
            } else {
                if (chdir(args[1]) != 0) {
                    perror("myshell");
                }
            }
            continue;
        }

	pid_t pid = fork();

	if(pid == 0) {
	    execvp(args[0], args);
	    perror("myshell");
	    exit(EXIT_FAILURE);
	}
	else if (pid < 0) {
	    perror("myerror");
	}
	else {
	    wait(NULL);
	}
 
    }

    return 0;
}
