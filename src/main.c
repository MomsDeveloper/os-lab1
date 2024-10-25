#include <stdio.h>
#include <stdlib.h>
#include <time.h> 
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    if (argc < 2){
        fprintf(stderr, "Not enough arguments.");
        exit(EXIT_FAILURE);
    }

    double time_spent = 0.0;
    clock_t begin, end;
    begin = clock();

    pid_t pid = vfork(); 

    if (pid < 0) {
        perror("vfork failed");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) { 
        char *args[argc];
        for (int i = 1; i < argc; i++){
            args[i-1] = argv[i];
        }
        args[argc-1] = NULL;

        execvp(args[0], args); 
        perror("exec failed"); 
        _exit(EXIT_FAILURE);   
    } else { 
        int status;
        waitpid(pid, &status, 0); 
        end = clock();
        time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
        printf("Execution time: %f seconds\n", time_spent);
        printf("Command '%s' executed.\n", argv[1]);
    }   


    return 0;
}