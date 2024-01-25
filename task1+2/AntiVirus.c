#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct virus
{
    unsigned short SigSize;
    char virusName[16];
    unsigned char *sig;
} virus;

typedef struct link
{
    struct link *nextVirus;
    virus *vir;
} link;

/*part 1a*/
virus *readVirus(FILE *file)
{
    virus *v = malloc(sizeof(virus));
    if (fread(v, 18, 1, file) != 1)
    {
        free(v);
        return NULL;
    }
    v->sig = malloc(v->SigSize);
    if (fread(v->sig, v->SigSize, 1, file) != 1)
    {
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

/*part 1b*/
void list_print(link *virus_list, FILE *output)
{
    link *curr = virus_list;
    while (curr != NULL)
    {
        printVirus(curr->vir, output);
        curr = curr->nextVirus;
    }
}

link *list_append(link *virus_list, virus *data)
{
    link *new_link = malloc(sizeof(link));
    new_link->vir = data;
    new_link->nextVirus = NULL;

    if (virus_list == NULL)
    {
        return new_link;
    }

    link *curr = virus_list;
    while (curr->nextVirus != NULL)
    {
        curr = curr->nextVirus;
    }
    curr->nextVirus = new_link;

    return virus_list;
}

void list_free(link *virus_list)
{
    link *curr = virus_list;
    while (curr != NULL)
    {
        link *next = curr->nextVirus;
        free(curr->vir->sig);
        free(curr->vir);
        free(curr);
        curr = next;
    }
}

int main(int argc, char *argv[])
{
    link *virus_list = NULL;
    char buffer[256];
    int choice;

    while (1)
    {
        printf("1) Load signatures\n2) Print signatures\n3) Detect viruses\n4) Fix file\n5) Quit\n Option:");
        fgets(buffer, sizeof(buffer), stdin);
        sscanf(buffer, "%d", &choice);

        switch (choice)
        {
        case 1:
            printf("Enter the signature file name: ");
            fgets(buffer, sizeof(buffer), stdin);
            buffer[strcspn(buffer, "\n")] = 0;
            FILE *file = fopen(buffer, "r");
            if (file == NULL)
            {
                printf("Could not open file %s\n \n", buffer);
                break;
            }
            else
            {
                printf("\nfile: %s was loaded successfully.\n", buffer);
            }
            char magic[5];
            fread(magic, 4, 1, file);
            magic[4] = '\0';
            if (strcmp(magic, "VIRL") != 0 && strcmp(magic, "VIRB") != 0)
            {
                printf("Invalid file format\n");
                break;
            }
            virus *v;
            while ((v = readVirus(file)) != NULL)
            {
                virus_list = list_append(virus_list, v);
            }
            fclose(file);
            break;
        case 2:
            if (virus_list == NULL)
            {
                printf("No signatures loaded\n");
            }
            else
            {
                list_print(virus_list, stdout);
            }
            break;
        case 3:
            printf("Not implemented\n");
            break;
        case 4:
            printf("Not implemented\n");
            break;
        case 5:
            list_free(virus_list);
            return 0;
        default:
            printf("Invalid choice\n");
            break;
        }
    }
}

// int main(int argc, char *argv[])
// {
//     /*task 1a test*/
//     FILE *file = fopen(argv[1], "r");
//     if (file == NULL)
//     {
//         printf("Could not open file %s\n", argv[1]);
//         return 1;
//     }
//     char magic[5];
//     fread(magic, 4, 1, file);
//     magic[4] = '\0';
//     if (strcmp(magic, "VIRL") != 0 && strcmp(magic, "VIRB") != 0)
//     {
//         printf("Invalid file format\n");
//         return 1;
//     }
//     virus *v;
//     while ((v = readVirus(file)) != NULL)
//     {
//         printVirus(v, stdout);
//         free(v->sig);
//         free(v);
//     }
//     fclose(file);
//     return 0;
// }