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
struct stat st;
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
exit(0);
}
buffer[num_of_chars - 1] = '\0';

if (strcmp(buffer, "exit") == 0)
        {
            free(buffer);
            exit(0);
        }

new_args = command_spiltter(buffer);
if (new_args == NULL)
{
free(buffer);
return (0);
}
if (stat(new_args[0], &st) != 0)
new_args[0] = handle_command(new_args[0]);
_exec_it(new_args, envp);
}
return (0);
}
