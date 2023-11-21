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
int num_of_chars, status = 0;
(void)argc;
(void)argv;
while (1)
{
if (isatty(STDIN_FILENO))
write(STDIN_FILENO, "dias@yassine: $ ", 17);
num_of_chars = getline(&buffer, &buffer_size, stdin);
if (num_of_chars == -1)
{
free(buffer);
exit(status);
}
buffer[num_of_chars - 1] = '\0';

        if (strcmp(buffer, "exit") == 0)
        {
            free(buffer);
            exit(status);
        }
        if (strcmp(buffer, "env") == 0)
        {
            system("env");
            continue;
        }


    new_args = command_spiltter(buffer);
    if (strcmp(new_args[0], "exit") == 0)
    {
        if (new_args[1])
        {
            if (is_number(new_args[1]) == 0)
            {
                free(buffer);
                _free_t(new_args);
                exit(-133);
            }

            status = atoi(new_args[1]);
            free(buffer);
            _free_t(new_args);
            if (status < 0)
                exit(2);
            exit(status);
        
        }
    }
    
    if (new_args == NULL)
    {
        free(buffer);
        return (0);
    }
        status = _exec_it(new_args, envp);

}
    return (0);

}
