#include "ema-sort-int.h"
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void merge(int f, int f1, int f2, int k, int a1, int a2) {
  int i, j;

  ssize_t read_result_f1 = read(f1, &a1, sizeof(int));
  ssize_t read_result_f2 = read(f2, &a2, sizeof(int));

  while (read_result_f1 > 0 && read_result_f2 > 0) {
    i = 0;
    j = 0;
    while (i < k && j < k && read_result_f1 > 0 && read_result_f2 > 0) {
      if (a1 < a2) {
        write(f, &a1, sizeof(int));
        read_result_f1 = read(f1, &a1, sizeof(int));
        i++;
      } else {
        write(f, &a2, sizeof(int));
        read_result_f2 = read(f2, &a2, sizeof(int));
        j++;
      }
    }
    while (i < k && read_result_f1 > 0) {
      write(f, &a1, sizeof(int));
      read_result_f1 = read(f1, &a1, sizeof(int));
      i++;
    }
    while (j < k && read_result_f2 > 0) {
      write(f, &a2, sizeof(int));
      read_result_f2 = read(f2, &a2, sizeof(int));
      j++;
    }
  }
  while (read_result_f1 > 0) {
    write(f, &a1, sizeof(int));
    read_result_f1 = read(f1, &a1, sizeof(int));
  }
  while (read_result_f2 > 0) {
    write(f, &a2, sizeof(int));
    read_result_f2 = read(f2, &a2, sizeof(int));
  }
}

void simple_merging_sort(char *name) {
  int a1, a2, kol, k;
  kol = 0;

  int f, f1, f2;
  char f1_name[256];
  char f2_name[256];

  snprintf(f1_name, sizeof(f1_name), "%s_f1.bin", name);
  snprintf(f2_name, sizeof(f2_name), "%s_f2.bin", name);

  f = open(name, O_RDONLY | O_SYNC);
  if (f == -1) {
    printf("Such file doesn't exist %s\n", name);
    return;
  }

  off_t size = lseek(f, 0, SEEK_END);
  kol = size / sizeof(int);
  lseek(f, 0, SEEK_SET);

  k = 1;
  while (k < kol) {
    f = open(name, O_RDONLY | O_SYNC);
    f1 = open(f1_name, O_WRONLY | O_CREAT | O_TRUNC | O_SYNC, 0644);
    f2 = open(f2_name, O_WRONLY | O_CREAT | O_TRUNC | O_SYNC, 0644);

    ssize_t read_result = read(f, &a1, sizeof(int));

    while (read_result > 0) {
      for (int i = 0; i < k && read_result > 0; i++) {
        write(f1, &a1, sizeof(int));
        read_result = read(f, &a1, sizeof(int));
      }
      for (int j = 0; j < k && read_result > 0; j++) {
        write(f2, &a1, sizeof(int));
        read_result = read(f, &a1, sizeof(int));
      }
    }
    close(f1);
    close(f2);
    close(f);

    f = open(name, O_WRONLY | O_SYNC);
    f1 = open(f1_name, O_RDONLY | O_SYNC);
    f2 = open(f2_name, O_RDONLY | O_SYNC);

    merge(f, f1, f2, k, a1, a2);

    close(f2);
    close(f1);
    close(f);

    k = k * 2;
  }

  remove(f1_name);
  remove(f2_name);
}

#ifdef BENCH1_MAIN
int main(int argc, char *argv[]) {
  if (argc != 3) {
    printf("Usage: %s <number_of_iterations> <file_name>\n", argv[0]);
    return 1;
  }

  char *filename = argv[2];
  int number_of_iterations = atoi(argv[1]);

  for (int i = 0; i < number_of_iterations; i++) {
    simple_merging_sort(filename);
    printf("Iteration %d done\n", i);
  }
  return 0;
}
#endif // BENCH1_MAIN
