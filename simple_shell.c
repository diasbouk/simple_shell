#include "shell.h"

/**
 * main -  Main func
 * @argc: Arg count
 * @argv:  argv 
 * @envp: environnement vars
 * Return: 0 on success or another number .
*/
int main(int argc, char **argv, char **envp)
{
    char *buffer = NULL, **new_args = NULL;
    size_t buffer_size;
    int num_of_chars;

    (void)argc;
    (void)argv;

    while (1)
    {

        num_of_chars = getline(&buffer, &buffer_size, stdin);
        if (num_of_chars == -1)
        {
            free(buffer);
            exit(0);
        }
        buffer[num_of_chars - 1] = '\0';
        
        new_args = command_spiltter(buffer);
        
        if (new_args == NULL)
        {
            free(buffer);
            return(0);
        }
        _exec_it(new_args, envp);

    }

    return (0);
    

}