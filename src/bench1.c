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
    if ((f = fopen("data/myfile.bin", "rb")) == NULL)
    {
        printf("Such file doesn't exist");
    }
    else
    {
        fseek(f, 0, SEEK_END);
        kol = ftell(f) / 4;
        rewind(f);
        printf("%d", kol);
    }

    k = 1;
    while (k < kol)
    {
        f = fopen(name, "rb");
        f1 = fopen("data/f1.bin", "wb");
        f2 = fopen("data/f2.bin", "wb");
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
        f1 = fopen("data/f1.bin", "rb");
        f2 = fopen("data/f2.bin", "rb");
        
        merge(f, f1, f2, k, a1, a2);
        
        fclose(f2);
        fclose(f1);
        fclose(f);
        
        k = k*2;
    }
}

int main()
{
    char *filename = "./data/myfile.bin";
    simple_merging_sort(filename);
    return 0;
}