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

    if (fread(&v->SigSize, sizeof(unsigned short), 1, file) != 1)
    {
        free(v);
        return NULL;
    }

    fread(v->virusName, sizeof(char), 16, file);
    v->sig = malloc(sizeof(unsigned char) * v->SigSize);
    fread(v->sig, sizeof(unsigned char), v->SigSize, file);

    return v;
}

void printVirus(virus *v, FILE *output)
{
    fprintf(output, "Virus Name: %s\n", v->virusName);
    fprintf(output, "Signature Length: %u\n", v->SigSize);
    fprintf(output, "Signature (Hex): ");
    for (int i = 0; i < v->SigSize; ++i)
    {
        fprintf(output, "%02X ", v->sig[i]);
    }
    fprintf(output, "\n");
}

/*part 1b*/
typedef struct link
{
    struct link *nextVirus;
    virus *vir;
} link;

void list_print(link *virus_list, FILE *output)
{
    link *current = virus_list;
    while (current != NULL)
    {
        fprintf(output, "Virus Name: %s\n", current->vir->virusName);
        fprintf(output, "Signature Length: %u\n", current->vir->SigSize);
        fprintf(output, "Signature (Hex): ");
        for (int i = 0; i < current->vir->SigSize; ++i)
        {
            fprintf(output, "%02X ", current->vir->sig[i]);
        }
        fprintf(output, "\n\n");
        current = current->nextVirus;
    }
}

link *list_append(link *virus_list, virus *data)
{
    link *newLink = (link *)malloc(sizeof(link));
    if (newLink == NULL)
    {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }

    newLink->vir = data;
    newLink->nextVirus = NULL;

    if (virus_list == NULL)
    {
        return newLink;
    }

    link *current = virus_list;
    while (current->nextVirus != NULL)
    {
        current = current->nextVirus;
    }

    current->nextVirus = newLink;
    return virus_list;
}

void list_free(link *virus_list)
{
    link *current = virus_list;
    while (current != NULL)
    {
        link *next = current->nextVirus;
        free(current->vir->sig);
        free(current->vir);
        free(current);
        current = next;
    }
}

int main(int argc, char **argv)
{
    FILE *signaturesFile = fopen("/home/amit/Desktop/LabB/task1+2/signatures-B", "rb");
    if (signaturesFile == NULL)
    {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char magicNumber[4];
    fread(magicNumber, sizeof(char), 4, signaturesFile);
    magicNumber[4] = '\0';

    if (strcmp(magicNumber, "VIRL") != 0 && strcmp(magicNumber, "VIRB") != 0)
    {
        fprintf(stderr, "Error: Incorrect magic number\n");
        fclose(signaturesFile);
        exit(EXIT_FAILURE);
    }

    fseek(signaturesFile, 0, SEEK_SET); // Reset file position

    virus *v;
    while ((v = readVirus(signaturesFile)) != NULL)
    {
        printVirus(v, stdout); // Print to standard output
        free(v->sig);
        free(v);
    }

    fclose(signaturesFile);
    return 0;
}
