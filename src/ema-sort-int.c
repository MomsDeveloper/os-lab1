#define _GNU_SOURCE
#include "ema-sort-int.h"
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int open_file(char *name, int flags, mode_t permissions) {
  int f;
#ifdef __APPLE__
  if (permissions == 0) {
    f = open(name, flags);
  } else {
    f = open(name, flags, permissions);
  }
  fcntl(f, F_NOCACHE, 1);
#elif defined(__linux__)
  flags |= O_DIRECT;
  if (permissions == 0) {
    f = open(name, flags);

  } else {
    f = open(name, flags, permissions);
  }
#endif
  if (f == -1) {
    perror("Failed to open file");
    return -1;
  }
  return f;
}

#define BUFFER_SIZE 1024



int read_int(int f, int* buffer, int* a, size_t *i) {
  if (*i % BUFFER_SIZE == 0) {
    if (read(f, buffer, BUFFER_SIZE * sizeof(int)) < 0) {
      printf("read failed: i=%zu\n", *i);
      return -1;
    }
  }
  *a = buffer[*i % BUFFER_SIZE];
  *i = *i + 1;
  return 1;
}

void flash_int(int f, int* buffer){
  if (write(f, buffer, BUFFER_SIZE * sizeof(int)) < 0) {
    printf("write failed\n");
  }
}

void write_int(int f, int* buffer, int a, int *i) {
  buffer[*i % BUFFER_SIZE] = a;
  if (*i % BUFFER_SIZE == 0 && *i != 0) {
    flash_int(f, buffer);
  }
  *i = *i + 1;
}

void merge(int f, int f1, int f2, int k, int a1, int a2, size_t f1_size, size_t f2_size) {
  f1_size = f1_size + 1;
  f2_size = f2_size + 1;

  size_t i, j;

  size_t index_f1_read_buffer = 0;
  size_t index_f2_read_buffer = 0;

  size_t index_write_buffer = 0;

  int buffer_read_f1[BUFFER_SIZE];
  int buffer_read_f2[BUFFER_SIZE];

  int buffer_write[BUFFER_SIZE];

  size_t read_result_f1 = read_int(f1, buffer_read_f1, &a1, &index_f1_read_buffer);
  size_t read_result_f2 = read_int(f2, buffer_read_f2, &a2, &index_f2_read_buffer);

  while (index_f1_read_buffer < f1_size && index_f2_read_buffer < f2_size) {
    i = 0;
    j = 0;
    while (i < k && j < k && index_f1_read_buffer < f1_size && index_f2_read_buffer < f2_size) {
      if (a1 < a2) {
        write_int(f, buffer_write, a1, &index_write_buffer);
        read_result_f1 = read_int(f1, buffer_read_f1, &a1, &index_f1_read_buffer);
        i++;
      } else {
        write_int(f, buffer_write, a2, &index_write_buffer);
        read_result_f2 = read_int(f2, buffer_read_f2, &a2, &index_f2_read_buffer);
        j++;
      }
    }
    while (i < k && index_f1_read_buffer < f1_size) {
      write_int(f, buffer_write, a1, &index_write_buffer);
      read_result_f1 = read_int(f1, buffer_read_f1, &a1, &index_f1_read_buffer);
      i++;
    }
    while (j < k && index_f2_read_buffer < f2_size) {
      write_int(f, buffer_write, a2, &index_write_buffer);
      read_result_f2 = read_int(f2, buffer_read_f2, &a2, &index_f2_read_buffer);
      j++;
    }
  }
  while (index_f1_read_buffer < f1_size) {
    write_int(f, buffer_write, a1, &index_write_buffer);
    read_result_f1 = read_int(f1, buffer_read_f1, &a1, &index_f1_read_buffer);
  }
  while (index_f2_read_buffer < f2_size) {
    write_int(f, buffer_write, a2, &index_write_buffer);
    read_result_f2 = read_int(f2, buffer_read_f2, &a2, &index_f2_read_buffer);
  }

  flash_int(f, buffer_write);
}

void simple_merging_sort(char *name) {
  int a1, a2;
  size_t kol, k;

  int f, f1, f2;
  char f1_name[256];
  char f2_name[256];

  snprintf(f1_name, sizeof(f1_name), "%s_f1.bin", name);
  snprintf(f2_name, sizeof(f2_name), "%s_f2.bin", name);

  f = open_file(name, O_RDONLY, 0);

  off_t size = lseek(f, 0, SEEK_END);
  kol = size / sizeof(int);
  lseek(f, 0, SEEK_SET);

  k = 1;
  while (k < kol) {

    f = open_file(name, O_RDONLY, 0);
    f1 = open_file(f1_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    f2 = open_file(f2_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);

    size_t index_f_read_buffer = 0;

    size_t index_f1_write_buffer = 0;
    size_t index_f2_write_buffer = 0;

    int buffer_read[BUFFER_SIZE];
    int buffer_write_f1[BUFFER_SIZE];
    int buffer_write_f2[BUFFER_SIZE];

    size_t read_result = read_int(f, buffer_read, &a1, &index_f_read_buffer);

    while (index_f_read_buffer < kol + 1) {
      for (int i = 0; i < k && index_f_read_buffer < kol + 1; i++) {
        printf("a1: %d\n", a1);
        write_int(f1, buffer_write_f1, a1, &index_f1_write_buffer);
        read_result = read_int(f, buffer_read, &a1, &index_f_read_buffer);
        printf("a1: %d\n", a1);
      }
      for (int j = 0; j < k && index_f_read_buffer < kol + 1; j++) {
        printf("a2: %d\n", a1);
        write_int(f2, buffer_write_f2, a1, &index_f2_write_buffer);
        read_result = read_int(f, buffer_read, &a1, &index_f_read_buffer);
        printf("a2: %d\n", a1);
      }
      
    }
    flash_int(f1, buffer_write_f1);
    flash_int(f2, buffer_write_f2);

    close(f1);
    close(f2);
    close(f);

    // f = open_file(name, O_WRONLY, 0);
    // f1 = open_file(f1_name, O_RDONLY, 0);
    // f2 = open_file(f2_name, O_RDONLY, 0);

    // merge(f, f1, f2, k, a1, a2, index_f1_write_buffer, index_f2_write_buffer);

    // close(f2);
    // close(f1);
    // close(f);

    // k = k * 2;
    k = 10000000;
  }

  // remove(f1_name);
  // remove(f2_name);
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
