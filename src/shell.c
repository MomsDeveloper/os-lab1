#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

void execute_command(char *command, char *args[]) {
  struct timespec begin, end;
  clock_gettime(CLOCK_MONOTONIC, &begin);

  pid_t pid = vfork();

  if (pid < 0) {
    perror("vfork failed");
    exit(EXIT_FAILURE);
  }

  if (pid == 0) {
    execvp(command, args);
    perror("exec failed");
    _exit(EXIT_FAILURE);
  } else {
    int status;
    waitpid(pid, &status, 0);

    clock_gettime(CLOCK_MONOTONIC, &end);
    double time_spent =
        (end.tv_sec - begin.tv_sec) + (end.tv_nsec - begin.tv_nsec) / 1e9;
    printf("Execution time: %f seconds\n", time_spent);
  }
}

int main() {
  char input[256];

  while (1) {
    printf("> ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
      perror("Error reading input");
      continue;
    }

    input[strcspn(input, "\n")] = '\0';

    if (strcmp(input, "exit") == 0) {
      break;
    }

    char *args[50];
    char *token = strtok(input, " ");
    int i = 0;

    while (token != NULL) {
      args[i++] = token;
      token = strtok(NULL, " ");
    }
    args[i] = NULL;

    execute_command(args[0], args);
  }

  return 0;
}
