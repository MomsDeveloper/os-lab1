#include "bench1.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void merge(FILE *f, FILE *f1, FILE *f2, int k, int a1, int a2)
{
    int i, j;

    if (!feof(f1))
        fread(&a1, sizeof(int), 1, f1);
    if (!feof(f2))
        fread(&a2, sizeof(int), 1, f2);

    while (!feof(f1) && !feof(f2))
    {
        i = 0;
        j = 0;
        while (i < k && j < k && !feof(f1) && !feof(f2))
        {
            if (a1 < a2)
            {
                fwrite(&a1, sizeof(int), 1, f);
                fread(&a1, sizeof(int), 1, f1);
                i++;
            }
            else
            {
                fwrite(&a2, sizeof(int), 1, f);
                fread(&a2, sizeof(int), 1, f2);
                j++;
            }
        }
        while (i < k && !feof(f1))
        {
            fwrite(&a1, sizeof(int), 1, f);
            fread(&a1, sizeof(int), 1, f1);
            i++;
        }
        while (j < k && !feof(f2))
        {
            fwrite(&a2, sizeof(int), 1, f);
            fread(&a2, sizeof(int), 1, f2);
            j++;
        }
    }
    while (!feof(f1))
    {
        fwrite(&a1, sizeof(int), 1, f);
        fread(&a1, sizeof(int), 1, f1);
    }
    while (!feof(f2))
    {
        fwrite(&a2, sizeof(int), 1, f);
        fread(&a2, sizeof(int), 1, f2);
    }
}

void simple_merging_sort(char *name)
{
    int a1, a2, kol, k;
    kol = 0;
    

    FILE *f, *f1, *f2;
    char f1_name[256];
    char f2_name[256];

    snprintf(f1_name, sizeof(f1_name), "%s_f1.bin", name);
    snprintf(f2_name, sizeof(f2_name), "%s_f2.bin", name);

    if ((f = fopen(name, "rb")) == NULL)
    {
        printf("Such file doesn't exist %s\n", name);
    }
    else
    {
        fseek(f, 0, SEEK_END);
        kol = ftell(f) / 4;
        rewind(f);
    }

    k = 1;
    while (k < kol)
    {
        f = fopen(name, "rb");
        f1 = fopen(f1_name, "wb");
        f2 = fopen(f2_name, "wb");

        if (!feof(f))
        {
            fread(&a1, sizeof(int), 1, f);
        }
        while (!feof(f))
        {
            for (int i = 0; i < k && !feof(f); i++)
            {
                fwrite(&a1, sizeof(int), 1, f1);
                fread(&a1, sizeof(int), 1, f);
            }
            for (int j = 0; j < k && !feof(f); j++)
            {
                fwrite(&a1, sizeof(int), 1, f2);
                fread(&a1, sizeof(int), 1, f);
            }
        }
        fclose(f1);
        fclose(f2);
        fclose(f);

        f = fopen(name, "wb");
        f1 = fopen(f1_name, "rb");
        f2 = fopen(f2_name, "rb");

        merge(f, f1, f2, k, a1, a2);

        fclose(f2);
        fclose(f1);
        fclose(f);

        k = k * 2;
    }

    remove(f1_name);
    remove(f2_name);


}

#ifdef BENCH1_MAIN
int main(int argc, char *argv[])
{   
    if (argc != 3)
    {
        printf("Usage: %s <number_of_iterations> <file_name>\n", argv[0]);
        return 1;
    }

    char *filename = argv[2];
    int number_of_iterations = atoi(argv[1]);

    for (int i = 0; i < number_of_iterations; i++)
    {
        simple_merging_sort(filename);
        printf("Iteration %d done\n", i);
    }
    return 0;
}
#endif // BENCH1_MAIN
