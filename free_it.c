#include "shell.h"

/**
 * _free_t - free a 2D array
 * @arr: array to free;
*/
void _free_t(char **arr)
{
    int i = 0;
    if (!arr)
        return;
    while (arr[i] != NULL)
    {
        free(arr[i]);
        arr[i] = NULL;
        i++;
    }
    free(arr);
    arr = NULL;
}