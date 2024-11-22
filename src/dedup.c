#include "dedup.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void remove_duplicates(int *arr, int *kol) {
  size_t i, j, k;
  for (i = 0; i < *kol; i++) {
    for (j = i + 1; j < *kol;) {
      if (arr[i] == arr[j]) {
        for (k = j; k < *kol; k++) {
          arr[k] = arr[k + 1];
        }
        (*kol)--;
      } else {
        j++;
      }
    }
  }
}

void generate_array(int *arr, int *kol) {
  for (size_t i = 0; i < *kol; i++) {
    arr[i] = rand() % 100;
  }
}

#ifdef BENCH2_MAIN
int main(int argc, char *argv[]) {
  if (argc != 3) {
    printf("Usage: %s <number_of_iterations> <number_of_kols>\n", argv[0]);
    return 1;
  }

  size_t kol = atoi(argv[2]);
  int number_of_iterations = atoi(argv[1]);

  int *arr = (int *)malloc(kol * sizeof(int));
  if (arr == NULL) {
    perror("Failed to allocate memory");
    return 1;
  }

  for (int i = 0; i < number_of_iterations; i++) {
    generate_array(arr, &kol);
    size_t temp_kol = kol;
    remove_duplicates(arr, &temp_kol);
    printf("Iteration %d done\n", i);
  }

  free(arr);
  return 0;
}
#endif // BENCH2_MAIN
