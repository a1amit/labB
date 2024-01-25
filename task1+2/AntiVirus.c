#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct virus
{
    unsigned short SigSize;
    char virusName[16];
    unsigned char *sig;
} virus;

/*part 1a*/
virus *readVirus(FILE *file)
{
    virus *v = malloc(sizeof(virus));
    if (v == NULL)
    {
        printf("Failed to allocate memory for virus\n");
        return NULL;
    }
    if (fread(v, 18, 1, file) != 1)
    {
        printf("Failed to read virus from file\n");
        free(v);
        return NULL;
    }
    v->sig = malloc(v->SigSize);
    if (v->sig == NULL)
    {
        printf("Failed to allocate memory for virus signature\n");
        free(v);
        return NULL;
    }
    if (fread(v->sig, v->SigSize, 1, file) != 1)
    {
        printf("Failed to read virus signature from file\n");
        free(v->sig);
        free(v);
        return NULL;
    }
    return v;
}

void printVirus(virus *v, FILE *output)
{
    fprintf(output, "Virus name: %s\n", v->virusName);
    fprintf(output, "Virus size: %hu\n", v->SigSize);
    fprintf(output, "signature:\n");
    for (int i = 0; i < v->SigSize; i++)
    {
        fprintf(output, "%02X ", v->sig[i]);
    }
    fprintf(output, "\n \n");
}

int main(int argc, char **argv)
{
    FILE *file = fopen(argv[1], "r");
    if (file == NULL)
    {
        printf("Could not open file %s\n", argv[1]);
        return 1;
    }
    char magic[5];
    fread(magic, 4, 1, file);
    magic[4] = '\0';
    if (strcmp(magic, "VIRL") != 0 && strcmp(magic, "VIRB") != 0)
    {
        printf("Invalid file format\n");
        return 1;
    }
    virus *v;
    while ((v = readVirus(file)) != NULL)
    {
        printVirus(v, stdout);
        free(v->sig);
        free(v);
    }
    fclose(file);
    return 0;
}
