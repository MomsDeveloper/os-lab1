#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "Not enough arguments.");
    exit(EXIT_FAILURE);
  }

  struct timespec begin, end;
  clock_gettime(CLOCK_MONOTONIC, &begin);

  pid_t pid = vfork();

  if (pid < 0) {
    perror("vfork failed");
    exit(EXIT_FAILURE);
  }

  if (pid == 0) {
    char *args[argc];
    for (int i = 1; i < argc; i++) {
      args[i - 1] = argv[i];
    }
    args[argc - 1] = NULL;

    execvp(args[0], args);
    perror("exec failed");
    _exit(EXIT_FAILURE);
  } else {
    int status;
    waitpid(pid, &status, 0);
    clock_gettime(CLOCK_MONOTONIC, &end);

    double time_spent =
        (end.tv_sec - begin.tv_sec) + (end.tv_nsec - begin.tv_nsec) / 1e9;
    printf("Execution time: %f seconds\n", time_spent);
    printf("Command '%s' executed.\n", argv[1]);
  }

  return 0;
}