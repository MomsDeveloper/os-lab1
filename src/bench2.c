#include "bench2.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void remove_duplicates(int *arr, int kol)
{
    int i, j, k;
    for (i = 0; i < kol; i++)
    {
        for (j = i + 1; j < kol;)
        {
            if (arr[i] == arr[j])
            {
                for (k = j; k < kol; k++)
                {
                    arr[k] = arr[k + 1];
                }
                kol--;
            }
            else
            {
                j++;
            }
        }
    }
}

void deduplicate_array(char *name)
{
    int kol = 0;

    FILE *f;
    if ((f = fopen(name, "rb")) == NULL)
    {
        printf("Such file doesn't exist");
    }
    else
    {
        fseek(f, 0, SEEK_END);
        kol = ftell(f) / 4;
        rewind(f);
        printf("File len is %d symbols\n", kol);
    }

    int *arr = (int *)malloc(kol * sizeof(int));
    if (arr == NULL)
    {
        printf("Memory allocation error\n");
        exit(1);
    }

    fread(arr, sizeof(int), kol, f);
    fclose(f);

    remove_duplicates(arr, kol);

    f = fopen(name, "wb");
    fwrite(arr, sizeof(int), kol, f);
    fclose(f);

    free(arr);
    

}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s <number_of_iterations>\n", argv[0]);
        return 1;
    }

    char *filename = "./data/mydupfile.bin";
    int number_of_iterations = atoi(argv[1]);

    for (int i = 0; i < number_of_iterations; i++)
    {
        deduplicate_array(filename);
    }

    return 0;
}