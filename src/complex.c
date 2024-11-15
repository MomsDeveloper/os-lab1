#include "ema-sort-int.h"
#include "dedup.h"
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void *run_bench1(void *arg) {
  char *name = (char *)arg;
  simple_merging_sort(name);
  return NULL;
}

typedef struct {
  int *arr;
  int kol;
} bench2_args;

void *run_bench2(void *arg) {
  bench2_args *args = (bench2_args *)arg;
  generate_array(args->arr, (int *)&(args->kol));
  remove_duplicates(args->arr, (int *)&(args->kol));
  return NULL;
}

int main(int argc, char *argv[]) {
  if (argc < 4) {
    printf("Usage: %s <number_of_threads> <number_of_kols> <file_name> "
           "<file_name> ...\n",
           argv[0]);
    return 1;
  }

  size_t kol = atoi(argv[2]);
  int number_of_threads = atoi(argv[1]);
  int *arr = (int *)malloc(kol * sizeof(int));
  if (arr == NULL) {
    perror("Failed to allocate memory");
    return 1;
  }

  for (int i = 0; i < number_of_threads; i++) {

    char *filename = argv[i + 3];

    pthread_t thread1, thread2;
    bench2_args args = {arr, kol};
    pthread_create(&thread2, NULL, run_bench2, &args);
    pthread_create(&thread1, NULL, run_bench1, filename);
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    printf("Thread %d finished\n", i);
  }

  free(arr);
  return 0;
}
