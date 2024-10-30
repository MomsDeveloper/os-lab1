#include "bench1.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define BLOCK_SIZE 2 

void swap (int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}



void sort_array(FILE *filestream) {

    int *elements;
    elements = (int *)malloc(2 * sizeof(int));

    if (elements == NULL) {
        fprintf(stderr, "\nError allocating memory\n");
        exit(1);
    }

    int values_count;
    fseek(filestream, 0, SEEK_END);
    values_count = ftell(filestream) / 4;
    rewind(filestream);

    if (values_count < 2) {
        fprintf(stderr, "\nNot enough values in file\n");
        exit(1);
    }

    for (int i = 0; i < values_count - 1; i++) {
        for (int j = 0; j < values_count - i - 1; j++) {
            fseek(filestream, j * sizeof(int), SEEK_SET);
            fread(elements, sizeof(int), 2, filestream);
            if (elements[0] > elements[1]) {
                swap(&elements[0], &elements[1]);

                fseek(filestream, j * sizeof(int), SEEK_SET);
                fwrite(elements, sizeof(int), 2, filestream);
            }

        }

    }

    free(elements);

    for (int i = 0; i < values_count; i++) {
        fseek(filestream, i * sizeof(int), SEEK_SET);
        fread(elements, sizeof(int), 1, filestream);
        printf("%d\n", elements[0]);
    }
}



// void bubbleSort(

int main(){
    FILE *filestream;
    filestream = fopen("./data/myfile.bin", "rb+");
    if (filestream == NULL) {
        fprintf(stderr, "\nError opening file\n");
        exit(1);
    }

    sort_array(filestream);

    fclose(filestream);
    return 0;
}