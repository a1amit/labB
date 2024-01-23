#include <stdio.h>
#include <stdlib.h>

void bubbleSort(int numbers[], int array_size)
{
    int i, j;
    int *temp;
    for (i = (array_size - 1); i > 0; i--)
    {
        for (j = 0; j < i; j++)  // Fix: Change 'j = 1' to 'j = 0'
        {
            if (numbers[j] > numbers[j + 1])  // Fix: Change 'numbers[j - 1] > numbers[j]' to 'numbers[j] > numbers[j + 1]'
            {
                temp = (int *)malloc(sizeof(int));  // Fix: Remove the pointer '*' from 'sizeof(int *)' to 'sizeof(int)'
                *temp = numbers[j];
                numbers[j] = numbers[j + 1];
                numbers[j + 1] = *temp;
                free(temp); // Fix: Freed temp after use
            }
        }
    }
}

int main(int argc, char **argv)
{
    char **arr = argv + 1;
    int i, n = argc - 1;
    int *numbers = (int *)calloc(n, sizeof(int));

    printf("Original array:");
    for (i = 0; i < n; ++i)  // Fix: Change 'i <= n' to 'i < n'
    {
        printf(" %s", arr[i]);
        numbers[i] = atoi(arr[i]);
    }
    printf("\n");

    bubbleSort(numbers, n);

    printf("Sorted array:");
    for (i = 0; i < n; ++i)
        printf(" %d", numbers[i]);
    printf("\n");

    // Fix: Free the allocated memory
    free(numbers);

    return 0;
}
